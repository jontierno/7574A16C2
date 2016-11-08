package ar.uba.fi.distribuidos1.cliente;

import ar.uba.fi.distribuidos1.commons.Constantes;
import com.rabbitmq.client.*;

import java.io.IOException;
import java.util.Scanner;
import java.util.UUID;

public class CajeroProducer {


    private static final String uuid = UUID.randomUUID().toString();
    private static final String MOVEMENT_TEMPLATE = "Tipo: %s       Monto: %s\n";

    private static String[] send(Channel ch, QueueingConsumer consumer, String message) throws IOException, InterruptedException {
        ch.basicPublish(Constantes.EXCHANGE_NAME, Constantes.ROUTING_KEY_OP,
                null, message.getBytes());
        QueueingConsumer.Delivery delivery = consumer.nextDelivery();
        String answ = new String(delivery.getBody());
        return answ.split(Constantes.MESSAGE_DELIMITER);
    }

    private static void checkInsuficiente(Integer id, String code) {
        if (Constantes.ANSWER_CODE_SALDO_INSUF.equals(code)) {
            System.out.printf("La cuenta %d no tiene saldo suficiente\n", id);
        }
    }

    private static void checkSuccess(String anObject, String format) {
        if (Constantes.ANSWER_CODE_SUCCESS.equals(anObject)) {
            System.out.println(format);
        }
    }

    private static void checkInexistente(Integer id, String code) {
        if (Constantes.ANSWER_CODE_CUENTA_INEXISTENTE.equals(code)) {
            System.out.printf("La cuenta %d no existe\n", id);
        }
    }

    private static void checkIOError(String code) {
        if (Constantes.ANSWER_CODE_IO_ERROR.equals(code)) {
            System.out.printf("Error de entrada salida en server.\n");
        }
    }

    static void crearCuenta(Channel ch, QueueingConsumer consumer, Integer id) {

        String message = String.format(Constantes.MESSAGE_ONLY_ACCOUNT_TEMP, uuid, Constantes.OPERACION_CREAR, id);

        try {
            String[] split = send(ch, consumer, message);
            checkSuccess(split[0], String.format("La cuenta %d fue creada exitosamente", id));
            if (Constantes.ANSWER_CODE_CUENTA_EXISTENTE.equals(split[0])) {
                System.out.printf("La cuenta %d ya existe\n", id);
            }
            checkIOError(split[0]);
        } catch (IOException | ShutdownSignalException | ConsumerCancelledException | InterruptedException e) {
            e.printStackTrace();
        }

    }

    static void consultarSaldo(Channel ch, QueueingConsumer consumer, Integer id) {
        String message = String.format(Constantes.MESSAGE_ONLY_ACCOUNT_TEMP, uuid, Constantes.OPERACION_SALDO, id);
        try {
            String[] split = send(ch, consumer, message);
            checkSuccess(split[0], String.format("La cuenta %d tiene un saldo de %s", id, split[1]));
            checkInexistente(id, split[0]);
            checkIOError(split[0]);
        } catch (IOException | ShutdownSignalException | ConsumerCancelledException | InterruptedException e) {
            e.printStackTrace();
        }
    }


    private static void generarMovimiento(Channel ch, QueueingConsumer consumer, Integer id, String message) {
        try {
            String[] split = send(ch, consumer, message);
            checkSuccess(split[0], String.format("Operacion exitosa. La cuenta %d tiene un saldo de %s", id, split[1]));
            checkInexistente(id, split[0]);
            checkIOError(split[0]);
            checkInsuficiente(id, split[0]);
        } catch (IOException | ShutdownSignalException | ConsumerCancelledException | InterruptedException e) {
            e.printStackTrace();
        }
    }



    static void depositar(Channel ch, QueueingConsumer consumer, Integer id, Integer cant) {

        String message = String.format(Constantes.MESSAGE_ACC_AM_TEMP, uuid, Constantes.OPERACION_DEPOSITAR, id, cant);
        generarMovimiento(ch, consumer, id, message);

    }

    static void retirar(Channel ch, QueueingConsumer consumer, Integer id, Integer cant) {
        String message = String.format(Constantes.MESSAGE_ACC_AM_TEMP, uuid, Constantes.OPERACION_EXTRAER, id, cant);
        generarMovimiento(ch, consumer, id, message);
    }

    static void movimientos(Channel ch, QueueingConsumer consumer, Integer id) {
        String message = String.format(Constantes.MESSAGE_ONLY_ACCOUNT_TEMP, uuid, Constantes.OPERACION_MOVIMIENTOS, id);
        try {
            String[] split = send(ch, consumer, message);
            checkInexistente(id, split[0]);
            checkIOError(split[0]);
            if (Constantes.ANSWER_CODE_SUCCESS.equals(split[0])) {
                String movimientos[] = split[1].split(Constantes.MOVEMENT_DELIMITER);
                for (int i = 0; i < movimientos.length; i++) {
                    String fields[] = movimientos[i].split(Constantes.MOVEMENTE_INNER_DELIM);
                    System.out.printf   (MOVEMENT_TEMPLATE, fields[0], fields[1]);
                }
            }

        } catch (IOException | ShutdownSignalException | ConsumerCancelledException | InterruptedException e) {
            e.printStackTrace();
        }

    }

    public static void main(String[] args) throws Exception {
        ConnectionFactory factory = new ConnectionFactory();
        factory.setHost("localhost");
        factory.setPort(5672);
        Connection conn = factory.newConnection();

        Channel channel = conn.createChannel();
        //cola de entrada
        String answerLabel = String.format(Constantes.ANSWER_ROUTING_TEMP, uuid);
        String queeueRetName = String.format(Constantes.ANSWER_QUEUE_NAME, uuid);
        String myExchange = Constantes.EXCHANGE_NAME + uuid;
        channel.exchangeDeclare(myExchange, "direct", false, true, null);
        channel.queueDeclare(queeueRetName, false, true, true, null);
        channel.queueBind(queeueRetName, myExchange, answerLabel);
        QueueingConsumer consumer = new QueueingConsumer(channel);
        channel.basicConsume(queeueRetName, true, consumer);

        Boolean continuar = true;
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
        channel.exchangeDelete(myExchange);
        channel.queueDelete(queeueRetName);
        scanIn.close();
        channel.close();

        conn.close();
    }
}
