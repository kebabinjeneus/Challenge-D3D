package API;

import com.google.gson.Gson;

import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

import static API.reader.readIn;

public class mockDatagetter {
    /**
     * Schrijft iedere 10 seconden het volgende datapunt naar de currentDataPoint
     * @param args commandline argumenten
     */
    public static void main(String[] args) {
        ArrayList<meterData> all = getData();
        System.out.println("The program will run for " + all.size()/6 + " minutes");
        for (meterData dataPoint : all) {
            jsonWriter(dataPoint);

            wait10();
        }
    }

    /**
     * Schrijft het object meterData naar het Json bestand genaamd currentDataPoint.json
     * @param dataPoint het datapunt om naar het Json bestand te schrijven
     */
    private static void jsonWriter(meterData dataPoint) {
        Gson gson = new Gson();
        try {
            String jsonString = gson.toJson(dataPoint);
            FileWriter writer = new FileWriter(
                    "C:\\Users\\Lars\\Documents\\Challenge-D3D\\src\\resources\\currentDataPoint.json"
            ); // TODO pad relatief maken en rootpad aan systeem vragen
            writer.write(jsonString);
            writer.close();
            System.out.printf("%td/%tm/%tY - %tH:%tM:%tS|| Point written || {Gas: %f, Verbruik laag: %f, Verbruik hoog: %f, Huidig verbruik: %f}\n",
                    System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(),
                    dataPoint.getGasverbruik(), dataPoint.getDagVerbruikLaag(), dataPoint.getDagVerbruikHoog(), dataPoint.getHuidigVerbruik());
        } catch (IOException e) {
            System.out.println("exception " + e.getMessage());
            e.printStackTrace();
        }
    }


    /**
     * Methode roept data per datapunt aan en propt het in een lijst
     * @return een ArrayList met meterData objecten, dit is alle data van de meter vertaald in objecten
     */
    static ArrayList<meterData> getData() { // todo make private
        ArrayList<meterData> out = new ArrayList<meterData>();
        int line = 31;
        for(int i = 0; true; i++) {
            out.add(readIn(line));
            line += 26;
            if(line > 18400) {
                break;
            }
        }
        return out;
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
