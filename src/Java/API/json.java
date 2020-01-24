package API;

import com.google.gson.Gson;

import java.io.FileWriter;
import java.io.IOException;

public class json {

    /**
     * Schrijft het object meterData naar het Json bestand genaamd currentDataPoint.json
     * @param dataPoint het datapunt om naar het Json bestand te schrijven
     */
    public static void jsonWriter(meterData dataPoint) {
        Gson gson = new Gson();
        try {
            String jsonString = gson.toJson(dataPoint);
            FileWriter writer = new FileWriter(
                    System.getProperty("user.dir") + "\\resources\\\\currentDataPoint.json"     // TODO: /var/www/html/currentDataPoint
            );
            writer.write(jsonString);
            writer.close();
            System.out.printf("%td/%tm/%tY - %tH:%tM:%tS || Point written to json || {Gas: %f, Verbruik laag: %f, Verbruik hoog: %f, Huidig verbruik: %f}\n",
                    System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(),
                    dataPoint.getGasVerbruik(), dataPoint.getTotVerbruikLaag(), dataPoint.getTotVerbruikHoog(), dataPoint.getHuidigVerbruik());
        } catch (IOException e) {
            System.out.println("exception " + e.getMessage());
            e.printStackTrace();
        }
    }


}
