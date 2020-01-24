import API.reader;
import API.meterData;

import static API.inputQuerys.dbFiller;

public class Main {
    public static void main(String[] args) {
        reader read = new reader();
        while(read.getCurrentLine() < 18400) {
            meterData dataPoint = read.getNextDatapoint();
            dbFiller(dataPoint);
            System.out.println(dataPoint.toString());
            wait10();
        }
    }


    /**
     * Laat het programma 10 seconden wachten
     */
    private static void wait10() {
        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
