package ar.uba.fi.distribuidos1.cajero.interf;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;

import ar.uba.fi.distribuidos1.cajero.interf.cuenta.CuentaInexistenteException;
import ar.uba.fi.distribuidos1.cajero.interf.operacion.Operacion;

public interface CajeroRemote extends Remote{

	Integer operar(Integer id, Operacion op) throws RemoteException;
	Integer crear(Integer id) throws RemoteException,CuentaInexistenteException;
	Integer consultaSaldo(Integer idCuenta) throws RemoteException;
	List<Movimiento> consultarMovimientos (Integer idCuenta) throws RemoteException;	
	
}
