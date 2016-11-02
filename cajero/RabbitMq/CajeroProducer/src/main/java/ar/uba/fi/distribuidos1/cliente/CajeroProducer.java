package ar.uba.fi.distribuidos1.cliente;

import ar.uba.fi.distribuidos1.commons.Constantes;
import com.rabbitmq.client.*;

import java.io.IOException;
import java.util.Scanner;
import java.util.UUID;

public class CajeroProducer {


    private static final UUID uuid = UUID.randomUUID();

    static void crearCuenta(Channel ch, QueueingConsumer consumer, Integer id) {

        String clientId = uuid.toString();
        String message = String.format(Constantes.MESSAGE_ONLY_ACCOUNT_TEMP, clientId, Constantes.OPERACION_CREAR, id);

        try {
            ch.basicPublish(Constantes.EXCHANGE_NAME, Constantes.ROUTING_KEY_OP,
                    null, message.getBytes());
            QueueingConsumer.Delivery delivery =  consumer.nextDelivery();
            String answ = new String(delivery.getBody());
            String[] split = answ.split(Constantes.MESSAGE_DELIMITER);
            if (Constantes.ANSWER_CODE_SUCCESS.equals(split[0])) {
                System.out.println(String.format("La cuenta %d fue creada exitosamente", id));
            }
            if (Constantes.ANSWER_CODE_CUENTA_EXISTENTE.equals(split[0])) {
                System.out.println(String.format("La cuenta %d ya existe", id));
            }
        } catch (IOException | ShutdownSignalException | ConsumerCancelledException | InterruptedException e) {
            e.printStackTrace();
        }

    }

    static void consultarSaldo(Channel ch, QueueingConsumer consumer, Integer id) {

        // Integer saldo = remote.consultaSaldo(id);
        // System.out.printf("La cuenta %d tiene un saldo de %d\n", id,saldo);

    }

    static void depositar(Channel ch, QueueingConsumer consumer, Integer id, Integer cant) {

        // Integer saldo = remote.operar(id, new Deposito(cant));
        // System.out.printf("Operacion exitosa. La cuenta %d tiene un saldo de %d\n",
        // id,saldo);

    }

    static void retirar(Channel ch, QueueingConsumer consumer, Integer id, Integer cant) {
        //
        // Integer saldo = remote.operar(id, new Extraccion(cant));
        // System.out.printf("Operacion exitosa. La cuenta %d tiene un saldo de %d\n",
        // id,saldo);
    }

    static void movimientos(Channel ch, QueueingConsumer consumer, Integer id) {

        // List<Movimiento> mvs = remote.consultarMovimientos(id);
        // for (Movimiento movimiento : mvs) {
        // System.out.printf("TIPO: %s        CANTIDAD: %d \n",movimiento.getTipo(),movimiento.getCantidad());
        // }
    }

    public static void main(String[] args) throws Exception {
        ConnectionFactory factory = new ConnectionFactory();
        factory.setHost("localhost");
        factory.setPort(5672);
        Connection conn = factory.newConnection();
        Channel channel = conn.createChannel();

        // Escucho los mensajes de respuesta hacia mí nada mas.
        String answerLabel = String.format(Constantes.ANSWER_ROUTING_TEMP, uuid.toString());

        String returnExchangeName = Constantes.EXCHANGE_NAME + uuid.toString();
        channel.exchangeDeclare(returnExchangeName, "direct", false, true, null);
        channel.queueBind(Constantes.QUEUE_NAME, returnExchangeName, answerLabel);
        QueueingConsumer consumer = new QueueingConsumer(channel);
        channel.basicConsume(Constantes.QUEUE_NAME, true, consumer);

        Boolean continuar= true;
        Scanner scanIn = new Scanner(System.in);
        while (continuar) {


            System.out.format("Ingrese cuenta: \n");
            Integer cuenta = Integer.parseInt(scanIn.nextLine());
            System.out.format("Ingrese operacion:\n1.CREAR\n2.SALDO\n3.DEPOSITAR\n4.RETIRAR\n5.MOVIMIENTOS\n6.SALIR\n");
            Integer operacion = Integer.parseInt(scanIn.nextLine());

            Integer cant = null;
            if (operacion == 3 || operacion == 4) {
                System.out.format("Ingrese Monto: \n");
                cant = Integer.parseInt(scanIn.nextLine());

            }

            switch (operacion) {
                case 1:
                    crearCuenta(channel, consumer, cuenta);
                    break;
                case 2:
                    consultarSaldo(channel, consumer, cuenta);
                    break;
                case 3:
                    depositar(channel, consumer, cuenta, cant);
                    break;
                case 4:
                    retirar(channel, consumer, cuenta, cant);
                    break;
                case 5:
                    movimientos(channel, consumer, cuenta);
                    break;
                case 6:
                    continuar = false;
                default:
                    break;
            }
        }
        channel.exchangeDelete(returnExchangeName);
        scanIn.close();
        channel.close();
        conn.close();
    }
}
