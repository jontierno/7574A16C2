package ar.uba.fi.distribuidos1.cajero;

import ar.uba.fi.distribuidos1.commons.*;
import com.rabbitmq.client.*;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.stream.Collectors;

public class CajeroConsumer {

    private static void validarCuenta(Integer idCuenta) {
        if (!Cuenta.exist(idCuenta)) {
            throw new CuentaInexistenteException();
        }
    }

    private static String crear(Integer idCuenta) {
        if (Cuenta.exist(idCuenta)) {
            return String.format(Constantes.ANSWER_CODE_CUENTA_EXISTENTE,
                    Constantes.ANSWER_CODE_IO_ERROR,
                    0);
        }
        try {
            new Cuenta(idCuenta);
            return String.format(Constantes.ANSWER_CODE_SUCCESS,
                    Constantes.ANSWER_CODE_IO_ERROR,
                    0);
        } catch (IOException e) {
            return String.format(Constantes.ANSWER_SIMPLE_TEMP,
                    Constantes.ANSWER_CODE_IO_ERROR,
                    0);
        }

    }


    private static String saldo(Integer idCuenta) {
        try {
            validarCuenta(idCuenta);
            Cuenta cuenta = new Cuenta(idCuenta);
            return String.format(Constantes.ANSWER_CODE_SUCCESS,
                    Constantes.ANSWER_CODE_SUCCESS,
                    cuenta.obtenerSaldo().toString());
        } catch (CuentaInexistenteException e) {
            return String.format(Constantes.ANSWER_SIMPLE_TEMP,
                    Constantes.ANSWER_CODE_CUENTA_INEXISTENTE,
                    0);
        } catch (IOException e) {
            return String.format(Constantes.ANSWER_SIMPLE_TEMP,
                    Constantes.ANSWER_CODE_IO_ERROR,
                    0);
        }
    }


    private static String movimientos(Integer idCuenta) {


        try {
            validarCuenta(idCuenta);
            Cuenta cuenta = new Cuenta(idCuenta);
            List<Movimiento> movimientos = cuenta.obtenerUltimosMovimientos(10);
            String result = movimientos
                    .stream()
                    .map(m -> m.getTipo() + Constantes.MOVEMENT_DELIMITER + m.getCantidad().toString())
                    .collect(Collectors.joining(Constantes.MESSAGE_DELIMITER));
            return String.format(Constantes.ANSWER_CODE_SUCCESS,
                    Constantes.ANSWER_CODE_SUCCESS,
                    result);
        } catch (CuentaInexistenteException e) {
            return String.format(Constantes.ANSWER_SIMPLE_TEMP,
                    Constantes.ANSWER_CODE_CUENTA_INEXISTENTE,
                    0);
        } catch (IOException e) {
            return String.format(Constantes.ANSWER_SIMPLE_TEMP,
                    Constantes.ANSWER_CODE_IO_ERROR,
                    0);
        }
    }

    private static String extraer(Integer idCuenta, Integer cantidad) {
        try {
            validarCuenta(idCuenta);
            Cuenta cuenta = new Cuenta(idCuenta);
            Extraccion e = new Extraccion(cantidad);
            e.operar(cuenta);
            return String.format(Constantes.ANSWER_CODE_SUCCESS,
                    Constantes.ANSWER_CODE_SUCCESS,
                    cuenta.obtenerSaldo().toString());
        } catch (CuentaInexistenteException e) {
            return String.format(Constantes.ANSWER_SIMPLE_TEMP,
                    Constantes.ANSWER_CODE_CUENTA_INEXISTENTE,
                    0);
        }catch (SaldoInsuficienteException e) {
            return String.format(Constantes.ANSWER_SIMPLE_TEMP,
                    Constantes.ANSWER_CODE_SALDO_INSUF,
                    0);
        } catch (IOException e) {
            return String.format(Constantes.ANSWER_SIMPLE_TEMP,
                    Constantes.ANSWER_CODE_IO_ERROR,
                    0);
        }

    }

    private static String depositar(Integer idCuenta, Integer cantidad) {
        try {
            validarCuenta(idCuenta);
            Cuenta cuenta = new Cuenta(idCuenta);
            Deposito d = new Deposito(cantidad);
            d.operar(cuenta);
            return String.format(Constantes.ANSWER_CODE_SUCCESS,
                    Constantes.ANSWER_CODE_SUCCESS,
                    cuenta.obtenerSaldo().toString());
        } catch (CuentaInexistenteException e) {
            return String.format(Constantes.ANSWER_SIMPLE_TEMP,
                    Constantes.ANSWER_CODE_CUENTA_INEXISTENTE,
                    0);
        } catch (IOException e) {
            return String.format(Constantes.ANSWER_SIMPLE_TEMP,
                    Constantes.ANSWER_CODE_IO_ERROR,
                    0);
        }
    }

    private static String operar(String[] operacion) {
        switch (operacion[1]) {
            case Constantes.OPERACION_CREAR:
                return crear(Integer.parseInt(operacion[2]));
            case Constantes.OPERACION_SALDO:
                return saldo(Integer.parseInt(operacion[2]));
            case Constantes.OPERACION_DEPOSITAR:
                return depositar(Integer.parseInt(operacion[2]), Integer.parseInt(operacion[3]));
            case Constantes.OPERACION_EXTRAER:
                return extraer(Integer.parseInt(operacion[2]), Integer.parseInt(operacion[3]));
            case Constantes.OPERACION_MOVIMIENTOS:
                return movimientos(Integer.parseInt(operacion[2]));
            default:
                break;
        }
        return String.format(Constantes.ANSWER_SIMPLE_TEMP, Constantes.ANSWER_CODE_SUCCESS, 0);
    }

    public static void main(String[] args) throws Exception {
        ConnectionFactory factory = new ConnectionFactory();
        factory.setHost("127.0.0.1");
        factory.setPort(5672);
        Connection conn = factory.newConnection();
        Channel channel = conn.createChannel();

        boolean durable = true;
        channel.exchangeDeclare(Constantes.EXCHANGE_NAME, "direct", durable);
        channel.queueDeclare(Constantes.QUEUE_NAME, durable, false, false, null);

        //Escucho todos los mensajes que sean una operacion.
        channel.queueBind(Constantes.QUEUE_NAME, Constantes.EXCHANGE_NAME, Constantes.ROUTING_KEY_OP);

        QueueingConsumer consumer = new QueueingConsumer(channel);
        System.out.println("Servidor listo para admitir mensajes");
        channel.basicConsume(Constantes.QUEUE_NAME, true, consumer);

        File theDir = new File(Cuenta.DB_DIRECTORY);

        // if the directory does not exist, create it
        if (!theDir.exists()) {
            boolean result = false;
            try {
                theDir.mkdir();
                result = true;
            } catch (SecurityException se) {
                //handle it
            }
            if (result) {
                System.out.println("Directorio de base de datos creado");
            }
        }
        boolean runInfinite = true;
        while (runInfinite) {
            QueueingConsumer.Delivery delivery;
            try {
                delivery = consumer.nextDelivery();
                String body = new String(delivery.getBody());
                System.out.format("Recibido %s\n", body);
                String[] split = body.split(Constantes.MESSAGE_DELIMITER);
                String routingKey = String.format(Constantes.ANSWER_ROUTING_TEMP, split[0]);
                String answer = operar(split);
                System.out.println(Constantes.EXCHANGE_NAME + split[0] + "\n");
                channel.basicPublish(Constantes.EXCHANGE_NAME + split[0], routingKey,
                        MessageProperties.PERSISTENT_TEXT_PLAIN, answer.getBytes());
            } catch (InterruptedException ie) {
                continue;
            }
        }
        channel.close();
        conn.close();
    }
}