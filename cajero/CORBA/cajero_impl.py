#!/usr/bin/env python

import sys
from omniORB import CORBA, PortableServer
import CosNaming, Example, Example__POA
import os
import constantes as ctes

BD_DIR = "bd"
SALDO_FILE = "bd/{}_saldo"
MOVIM_FILE = "bd/{}_movs"
OP_TEMPLATE = "{}, {}\n"

# Defino la implementación de la interfaz Cajero
class Cajero_i (Example__POA.Cajero):
    def crear(self, id):
        saldo_file = SALDO_FILE.format(id)
        mov_file = MOVIM_FILE.format(id)
        if os.path.exists(saldo_file):
            return Example.ResultadoOperacion (ctes.COD_CUENTA_EXISTENTE, 0)
        
        sf = open(saldo_file, 'a')
        open(mov_file, 'a').close()
        sf.write("0")
        sf.close
        return Example.ResultadoOperacion (ctes.COD_SUCCESS, 0)
    
    def saldo(self, id):
        saldo_file = SALDO_FILE.format(id)
        if not os.path.exists(saldo_file):
            return Example.ResultadoOperacion (ctes.COD_CUENTA_INEXISTENTE, 0)
        sf = open(saldo_file, 'r')
        saldo = sf.readline()
        return Example.ResultadoOperacion (ctes.COD_SUCCESS, int(saldo))
    
    def depositar(self, id, cantidad):
        saldo_file = SALDO_FILE.format(id)
        mov_file = MOVIM_FILE.format(id)
        if not os.path.exists(saldo_file):
            return Example.ResultadoOperacion (ctes.COD_CUENTA_INEXISTENTE, 0)
        sf = open(saldo_file, 'r+')
        saldo = int(sf.readline()) + cantidad
        sf.close()
        sf = open(saldo_file, 'w')
        sf.write(str(saldo))
        sf.close()
        mf = open(mov_file,'a')
        mf.write(OP_TEMPLATE.format(ctes.OP_DEPOSITO, cantidad))
        mf.close()
        return Example.ResultadoOperacion (ctes.COD_SUCCESS, saldo)
    
    def extraer(self, id, cantidad):
        saldo_file = SALDO_FILE.format(id)
        mov_file = MOVIM_FILE.format(id)
        if not os.path.exists(saldo_file):
            return Example.ResultadoOperacion (ctes.COD_CUENTA_INEXISTENTE, 0)
        sf = open(saldo_file, 'r+')
        saldo = int(sf.readline())
        sf.close()
        if saldo < cantidad:
           return Example.ResultadoOperacion (ctes.COD_SALDO_INSUF, saldo) 

        saldo = saldo -cantidad
        sf = open(saldo_file, 'w')
        sf.write(str(saldo))
        sf.close()
        mf = open(mov_file,'a')
        mf.write(OP_TEMPLATE.format(ctes.OP_EXTRACCION, cantidad))
        mf.close()
        return Example.ResultadoOperacion (ctes.COD_SUCCESS, saldo)

    def consultaMovimientos(self, id):
        a = []
        saldo_file = SALDO_FILE.format(id)
        mov_file = MOVIM_FILE.format(id)
        if not os.path.exists(saldo_file):
            return Example.Historial (ctes.COD_CUENTA_INEXISTENTE, a)
        mf = open(mov_file,'r')
        for line in mf:
            b = line.split(',')
            a.insert(0,Example.Operacion(b[0],long(b[1])))
        mf.close()
        
        return Example.Historial(ctes.COD_SUCCESS, a [0:10])

# Inicializar el ORB y buscar el root POA
orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)
poa = orb.resolve_initial_references("RootPOA")

# Crear una instancia de interfaz Cajero_i y la referencia al objeto Cajero
ei = Cajero_i()
eo = ei._this()

# Obtener una referencia al contexto de nombres root 
obj = orb.resolve_initial_references("NameService")
rootContext = obj._narrow(CosNaming.NamingContext)

if rootContext is None:
    print "Failed to narrow the root naming context"
    sys.exit(1)

# Bindeo del contexto "test.my_context" al contexto root
name = [CosNaming.NameComponent("test", "my_context")]

try:
    testContext = rootContext.bind_new_context(name)
    print "New test context bound"
except CosNaming.NamingContext.AlreadyBound, ex:
    print "Test context already exists"
    obj = rootContext.resolve(name)
    testContext = obj._narrow(CosNaming.NamingContext)
    if testContext is None:
        print "test.mycontext exists but is not a NamingContext"
        sys.exit(1)

# Bind del objeto Cajero al contexto de test
name = [CosNaming.NameComponent("ExampleCajero", "Object")]

try:
    testContext.bind(name, eo)
    print "New ExampleCajero object bound"
except CosNaming.NamingContext.AlreadyBound:
    testContext.rebind(name, eo)
    print "ExampleCajero binding already existed -- rebound"

# Activaar el POA
poaManager = poa._get_the_POAManager()
poaManager.activate()

if not os.path.exists(BD_DIR):
    print("El directorio BD no existe, sera creado")
    os.makedirs(BD_DIR)

# Corro el orb
orb.run()
