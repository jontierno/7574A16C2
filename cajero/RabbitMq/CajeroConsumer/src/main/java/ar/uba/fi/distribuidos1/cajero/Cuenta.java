package ar.uba.fi.distribuidos1.cajero;

import ar.uba.fi.distribuidos1.commons.Movimiento;

import java.io.*;
import java.util.LinkedList;
import java.util.List;


public class Cuenta {
	private static final String MOVIMIENTOS_TOKEN = ",";
	public static final String DB_DIRECTORY = "./bd";
	private static final String PREFIJO_ARCH_SALDO= DB_DIRECTORY +"/%d_saldo"; 
	private static final String PREFIJO_ARCH_MOVS= DB_DIRECTORY+"/%d_mov";
	private static final String MOV_TEMPLATE= "%s"+MOVIMIENTOS_TOKEN+"%d\n";
	private static final String EXTRAER_STR = "EXT";
	
	private static final String DEPOSITO_STR = "DEP";

	private File archSaldo;
	private File archMovs;
	
	
	public Cuenta(Integer idCuenta) throws IOException {
		archMovs = new File(String.format(PREFIJO_ARCH_MOVS, idCuenta));

		//creo los archivos de la cuenta.
		if(!archMovs.exists() || archMovs.isDirectory()) { 
				BufferedWriter bw = new BufferedWriter(new FileWriter(archMovs));
				bw.close();
		}

		
		archSaldo = new File(String.format(PREFIJO_ARCH_SALDO, idCuenta));
		if(!archSaldo.exists() || archSaldo.isDirectory()) { 
			BufferedWriter bw = new BufferedWriter(new FileWriter(archSaldo));
			bw.write("0");
			bw.close();
		}
	}

	public void depositar(Integer cantidad) throws IOException{
		Integer saldo = this.leerSaldo();
		saldo += cantidad;
		this.actualizarSaldo(saldo);
		this.agregarMovimiento(new Movimiento(DEPOSITO_STR,cantidad));
	}

	public void extraer(Integer cantidad) throws IOException {
		Integer saldo = this.leerSaldo();
		if(saldo > cantidad) {
			saldo -= cantidad;
			this.actualizarSaldo(saldo);
			this.agregarMovimiento(new Movimiento(EXTRAER_STR,cantidad));
		} else {
			throw new SaldoInsuficienteException();
		}
	}

	public List<Movimiento> obtenerUltimosMovimientos(Integer count) throws NumberFormatException, IOException {
		List<Movimiento> quee = new LinkedList<Movimiento>();
		
		try {
			BufferedReader bw = new BufferedReader(new FileReader(archMovs));
			String l;
			
			while( (l=bw.readLine()) != null){
				String[] split = l.split(MOVIMIENTOS_TOKEN);
				quee.add(new Movimiento(split[0], Integer.parseInt(split[1])));
			}
			bw.close();
			List<Movimiento> result = new LinkedList<Movimiento>();
			quee.stream().skip(Math.max(0, quee.size() - 10)).forEach((a)->result.add(0,a));
			return result;
			
		} catch (FileNotFoundException e) {
			throw new CuentaExistenteException();
		} 
	}
	
	public Integer obtenerSaldo() throws IOException {
		return this.leerSaldo();
	}
	
	
	public static boolean exist(Integer id) {
		File archc = new File(String.format(PREFIJO_ARCH_SALDO, id));
		if(archc.exists()) { 
		    return true;
		}
		return false;
	}
	
	
	private void actualizarSaldo(Integer saldo) throws IOException {
		try {
			BufferedWriter bw = new BufferedWriter(new FileWriter(archSaldo));
			bw.write(saldo.toString());
			bw.close();
		} catch (FileNotFoundException e) {
			throw new CuentaInexistenteException();
		}
		
	}
	
	private void agregarMovimiento(Movimiento m) throws IOException {
		try {
			BufferedWriter bw = new BufferedWriter(new FileWriter(archMovs,true));
			bw.append(String.format(MOV_TEMPLATE, m.getTipo(),m.getCantidad()));
			bw.close();
		} catch (FileNotFoundException e) {
			throw new CuentaInexistenteException();
		} 
		
	}
	
	private Integer leerSaldo() throws IOException{
		try {
			BufferedReader br = new BufferedReader(new FileReader(archSaldo));
			String ln = br.readLine();
			br.close();
			return Integer.parseInt(ln);
		} catch (FileNotFoundException e) {
			throw new CuentaInexistenteException();
		} 

	}
	
}
