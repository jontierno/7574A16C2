package ar.uba.fi.distribuidos1.commons;

import java.io.Serializable;

public class Movimiento implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 3191004045076918099L;
	private String tipo;
	private Integer cantidad;
	
	public Movimiento(String tipo, Integer cantidad) {
		this.tipo = tipo;
		this.cantidad = cantidad;
	}

	public String getTipo() {
		return tipo;
	}

	public void setTipo(String tipo) {
		this.tipo = tipo;
	}

	public Integer getCantidad() {
		return cantidad;
	}

	public void setCantidad(Integer cantidad) {
		this.cantidad = cantidad;
	}
	
	
}
