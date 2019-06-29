package sample;
import jssc.*;

public class Controller {
    public static void main(String[] args){
        // Test project JSSC
        SerialPort serialPort = new SerialPort("COM4");
        try{
            serialPort.openPort();
            serialPort.setParams(SerialPort.BAUDRATE_9600,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
            serialPort.addEventListener(new SerialPortEventListener() {
                @Override
                public void serialEvent(SerialPortEvent serialPortEvent) {

                }
            });
            for(;;){
                Thread.sleep(10);
                String buffer = serialPort.readString();
                System.out.println(buffer);
            }
        }catch (Exception e){
            e.printStackTrace();
        }

    }

}
