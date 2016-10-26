package ar.uba.fi.distribuidos1.cajero.client;

import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.List;
import java.util.Scanner;

import ar.uba.fi.distribuidos1.cajero.interf.Constant;
import ar.uba.fi.distribuidos1.cajero.interf.Movimiento;
import ar.uba.fi.distribuidos1.cajero.interf.CajeroRemote;
import ar.uba.fi.distribuidos1.cajero.interf.cuenta.CuentaExistenteException;
import ar.uba.fi.distribuidos1.cajero.interf.cuenta.CuentaInexistenteException;
import ar.uba.fi.distribuidos1.cajero.interf.cuenta.SaldoInsuficienteException;
import ar.uba.fi.distribuidos1.cajero.interf.operacion.Deposito;
import ar.uba.fi.distribuidos1.cajero.interf.operacion.Extraccion;

public class CajeroClient {

	
	static void crearCuenta(CajeroRemote remote, Integer id) {
		try {
			remote.crear(id);
			System.out.println("Cuenta creada con exito");
		}
		catch (RemoteException e) {
			//No maneja diferentes tipos de errores
			if(e.detail instanceof CuentaExistenteException){
				System.out.println("La cuenta ya existe");
			} else 
			System.out.println("La cuenta no pudo ser creada, error desconocido");
		}
	
	}
	
	static void consultarSaldo(CajeroRemote remote, Integer id) {
		try {
			Integer saldo = remote.consultaSaldo(id);
			System.out.printf("La cuenta %d tiene un saldo de %d\n", id,saldo);
		} catch (RemoteException e) {
			if(e.detail instanceof RemoteException && ((RemoteException)e.detail).detail instanceof CuentaInexistenteException ){
				System.out.printf("La cuenta %d no existe\n",id);
			} else 
			System.out.println("No se pudo consultar el saldo, error de comunicación");
		}
		
		
	}
	
	static void depositar(CajeroRemote remote, Integer id, Integer cant) {
		try {
			Integer saldo = remote.operar(id, new Deposito(cant));
			System.out.printf("Operacion exitosa. La cuenta %d tiene un saldo de %d\n", id,saldo);
		} catch (RemoteException e) {
			if(e.detail instanceof CuentaInexistenteException ){
				System.out.printf("La cuenta %d no existe\n",id);
			} else  {
				System.out.println("Fallo la operacion, error desconocido");
				e.printStackTrace();				
			}
		}
		
	}
	

	static void retirar(CajeroRemote remote, Integer id, Integer cant) {
		try {
			Integer saldo = remote.operar(id, new Extraccion(cant));
			System.out.printf("Operacion exitosa. La cuenta %d tiene un saldo de %d\n", id,saldo);
		}  catch (RemoteException e) {
			if(e.detail instanceof CuentaInexistenteException ){
				System.out.printf("La cuenta %d no existe\n",id);
				return;
			} 
			if(e.detail instanceof RemoteException && ((RemoteException)e.detail).detail instanceof SaldoInsuficienteException ){
				
				System.out.printf("El saldo de la cuenta %d es insuficiente\n",id);
				return;
			}
			
			
			System.out.println("Fallo la operacion, error desconocido");
			e.printStackTrace();
			
		}
	}

	static void movimientos(CajeroRemote remote, Integer id) {
		try {
			List<Movimiento> mvs = remote.consultarMovimientos(id);
			for (Movimiento movimiento : mvs) {
				System.out.printf("TIPO: %s        CANTIDAD: %d \n",movimiento.getTipo(),movimiento.getCantidad());
			}
		} catch (RemoteException e) {
			if(e.detail instanceof RemoteException && ((RemoteException)e.detail).detail instanceof CuentaInexistenteException){
				System.out.printf("La cuenta %d no existe\n",id);
				return;
			}
			System.out.println("Fallo la operacion, error desconocido");
			e.printStackTrace();
		}
		
	}

	public static void main(String[] args) throws RemoteException, NotBoundException {
		
				
				if (args.length< 1) {
					System.out.format("usage: %s server_host\n", "TestClient");
					System.exit(1);
				}
				
				Registry registry = LocateRegistry.getRegistry(args[0], Constant.RMI_PORT);
				CajeroRemote remote = (CajeroRemote) registry.lookup(Constant.RMI_ID);

				
				System.out.format("Ingrese cuenta: \n");
			    Scanner scanIn = new Scanner(System.in);
			       
				Integer cuenta = Integer.parseInt(scanIn.nextLine());
				
				System.out.format("Ingrese operacion CREAR(1), CONSULTA(2), DEPOSITAR(3), RETIRAR(4), MOVIMIENTOS(5): \n");

				Integer operacion = Integer.parseInt(scanIn.nextLine());
				
				scanIn.close();
				Integer cant = null;
				if(operacion == 3 || operacion == 4){
					System.out.format("Ingrese cantidad: \n");
					cant = Integer.parseInt(scanIn.nextLine());

				}


				switch (operacion) {
					case 1:
						crearCuenta(remote,cuenta);
						break;
					case 2:
							consultarSaldo(remote,cuenta);
						break;
					case 3:
						depositar(remote,cuenta,cant);
						break;
					case 4:
						retirar(remote,cuenta,cant);
						break;
					case 5:
						movimientos(remote,cuenta);
						break;
					default:
						break;
				}
			}



	}
