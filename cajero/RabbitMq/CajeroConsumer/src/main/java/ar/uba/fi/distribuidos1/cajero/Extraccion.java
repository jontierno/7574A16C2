package ar.uba.fi.distribuidos1.cajero;

import java.io.IOException;

public class Extraccion extends Operacion {


	private static final long serialVersionUID = -4970801838758847335L;

	public Extraccion(Integer cantidad) {
		super(cantidad);
	}

	@Override
	public void operar(Cuenta c) throws IOException{
		c.extraer(this.cantidad);
	}
}
