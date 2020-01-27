import API.Reader;
import API.MeterData;

import static API.InputQuerys.dbFiller;
import static API.Json.jsonWriter;
import static Properties.Store.initialiseerProperties;
import static Properties.Store.storeValue;

public class Main {
    public static void main(String[] args) {
        //initialiseerProperties();
        Reader read = new Reader();
        while(read.getCurrentLine() < 18400) {
            MeterData dataPoint = read.getNextDatapoint();
//            jsonWriter(dataPoint);
//            dbFiller(dataPoint);
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
