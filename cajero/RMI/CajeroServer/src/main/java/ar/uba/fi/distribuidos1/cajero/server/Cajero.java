package ar.uba.fi.distribuidos1.cajero.server;

import java.io.File;
import java.io.IOException;
import java.rmi.RemoteException;
import java.util.List;

import ar.uba.fi.distribuidos1.cajero.interf.Movimiento;
import ar.uba.fi.distribuidos1.cajero.interf.cuenta.Cuenta;
import ar.uba.fi.distribuidos1.cajero.interf.cuenta.CuentaExistenteException;
import ar.uba.fi.distribuidos1.cajero.interf.cuenta.CuentaInexistenteException;
import ar.uba.fi.distribuidos1.cajero.interf.operacion.Operacion;


public class Cajero {
	
	private static final int CANT_MOVIMIENTOS = 10;
	public Cajero() {
		File theDir = new File(Cuenta.DB_DIRECTORY);

		// if the directory does not exist, create it
		if (!theDir.exists()) {
		    boolean result = false;
		    try{
		        theDir.mkdir();
		        result = true;
		    } 
		    catch(SecurityException se){
		        //handle it
		    }        
		    if(result) {    
		        System.out.println("Directorio de base de datos creado");  
		    }
		}
	}

	
	
	public void operar(Integer idCuenta, Operacion op) throws RemoteException {
		validarCuenta(idCuenta);
		Cuenta cuenta;
		try {
			cuenta = new Cuenta(idCuenta);
			op.operar(cuenta);
		} catch (IOException e) {
			throw new RemoteException("Error I/O", e);
		}
		
	}


	
	public void crear(Integer idCuenta) throws RemoteException {
		if(Cuenta.exist(idCuenta)) {
			throw new CuentaExistenteException();
		}
		try {
			new Cuenta(idCuenta);
		} catch (IOException e) {
			throw new RemoteException("Error I/O", e);
		}
	}
	
	public List<Movimiento> getMovimientos(Integer idCuenta) throws RemoteException {
		try {
			validarCuenta(idCuenta);
			Cuenta cuenta = new Cuenta(idCuenta);
			return cuenta.obtenerUltimosMovimientos(CANT_MOVIMIENTOS);
		} catch (IOException e) {
			throw new RemoteException("Error I/O", e);
		}
		
	}



	private void validarCuenta(Integer idCuenta)
			throws CuentaInexistenteException {
		if(!Cuenta.exist(idCuenta)) {
			throw new CuentaInexistenteException();
		}
	}
	
	public Integer consultaSaldo(Integer idCuenta) throws RemoteException {
		try {
			validarCuenta(idCuenta);
			Cuenta cuenta = new Cuenta(idCuenta);
			return cuenta.obtenerSaldo();	
		} catch (IOException e) {
			throw new RemoteException("Error I/O", e);
		}
		
	}
	
}
