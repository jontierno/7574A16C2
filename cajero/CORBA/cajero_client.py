#!/usr/bin/env python

import sys
from omniORB import CORBA
import CosNaming, Example
import constantes as ctes

# Initialise the ORB
orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

# Obtain a reference to the root naming context
obj = orb.resolve_initial_references("NameService")
rootContext = obj._narrow(CosNaming.NamingContext)

if rootContext is None:
    print "Failed to narrow the root naming context"
    sys.exit(1)

# Resolve the name "test.my_context/ExampleEcho.Object"
name = [CosNaming.NameComponent("test", "my_context"), CosNaming.NameComponent("ExampleCajero", "Object")]
try:
    obj = rootContext.resolve(name)
except CosNaming.NamingContext.NotFound, ex:
    print "Name not found"
    sys.exit(1)

# Narrow the object to an Example::Cajero
cajero = obj._narrow(Example.Cajero)
if cajero is None:
    print "Object reference is not an Example::Cajero"
    sys.exit(1)

def ingresarCuenta():
  cont = True
  cuenta = 0
  while cont:
    try:
      cuenta=int(raw_input('Ingrese el numero de cuenta:'))
      cont = False
    except ValueError:
      print "Numero invalido"
  return cuenta

def ingresarCantidad():
  cont = True
  cant = 0
  while cont:
    try:
      cant=int(raw_input('Ingrese el monto:'))
      cont = False
    except ValueError:
      print "Monto invalido"
  return cant

# Operaciones sobre el cajero
def crear():
  cuenta = ingresarCuenta()
  ans = cajero.crear(cuenta)
  if ans.resultado == ctes.COD_SUCCESS:
    print("Cuenta creada exitosamente\n")
  if ans.resultado == ctes.COD_CUENTA_EXISTENTE:
    print("La cuenta ya existe\n")

def saldo():
  cuenta = ingresarCuenta()
  ans = cajero.saldo(cuenta)
  if ans.resultado == ctes.COD_SUCCESS:
    print("La cuenta {} tiene un saldo de {} \n".format(cuenta, ans.saldo))
  if ans.resultado == ctes.COD_CUENTA_INEXISTENTE:
    print("La cuenta {} no existe\n".format(cuenta))

def depositar():
   cuenta = ingresarCuenta()
   cantidad = ingresarCantidad()
   ans = cajero.depositar(cuenta,cantidad)
   if ans.resultado == ctes.COD_SUCCESS:
    print("La cuenta {} tiene un saldo de {} \n".format(cuenta, ans.saldo))
   if ans.resultado == ctes.COD_CUENTA_INEXISTENTE:
    print("La cuenta {} no existe\n".format(cuenta))
   print(ans)

def extraer():
   cuenta = ingresarCuenta()
   cantidad = ingresarCantidad()
   resultado = cajero.extraer(cuenta,cantidad)

def movimientos():
   cuenta = ingresarCuenta()
   resultado = cajero.movimientos(cuenta)

ans=True
while ans:
    print("""
    1.Crear Cuenta
    2.Consultar Saldo
    3.Depositar
    4.Extraer
    5.Consultar Movimientos
    6.Salir
    """)
    ans=raw_input("Ingrese la Opcion ")
    if ans=="1":
      crear()
      print ("\n")
    elif ans=="2":
      saldo()
      print ("\n")
    elif ans=="3":
      depositar()
      print ("\n")
    elif ans=="4":
      extraer()
      print ("\n")
    elif ans=="5":
      movimientos()
      print ("\n")
    elif ans=="6":
      print("\n Finalizando") 
      ans = None
    else:
       print("\n Opcion invalida")