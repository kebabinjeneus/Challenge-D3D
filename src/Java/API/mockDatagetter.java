package API;

import com.google.gson.Gson;

import java.io.FileWriter;
import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.ArrayList;

import static API.reader.readIn;
import static API.BasicQueries.closeCon;

public class mockDatagetter {
//    /**
//     * Schrijft iedere 10 seconden het volgende datapunt naar de currentDataPoint
//     * Schrijft tevens elke 10 seconden het komende datapunt naar de database
//     * @param args commandline argumenten
//     */
//    public static void main(String[] args) throws SQLException {
//        ArrayList<meterData> all = getData();
//        System.out.println("The program will run for " + all.size()/6 + " minutes");
//        for (meterData dataPoint : all) {
//            jsonWriter(dataPoint);
//            dbFiller(dataPoint);
//            wait10();
//        }
//    }

    /**
     * Schrijft het object meterData naar het Json bestand genaamd currentDataPoint.json
     * @param dataPoint het datapunt om naar het Json bestand te schrijven
     */
    private static void jsonWriter(meterData dataPoint) {
        Gson gson = new Gson();
        try {
            String jsonString = gson.toJson(dataPoint);
            FileWriter writer = new FileWriter(
                    System.getProperty("user.dir") + "\\src\\resources\\\\currentDataPoint.json"
            );
            writer.write(jsonString);
            writer.close();
            System.out.printf("%td/%tm/%tY - %tH:%tM:%tS || Point written to json || {Gas: %f, Verbruik laag: %f, Verbruik hoog: %f, Huidig verbruik: %f}\n",
                    System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(), System.currentTimeMillis(),
                    dataPoint.getGasverbruik(), dataPoint.getTotVerbruikLaag(), dataPoint.getTotVerbruikHoog(), dataPoint.getHuidigVerbruik());
        } catch (IOException e) {
            System.out.println("exception " + e.getMessage());
            e.printStackTrace();
        }
    }

    /**
     * @param dataPoint het huidige datapunt dat in de database geyeet dient te worden.
     * @throws SQLException
     */
    private static void dbFiller(meterData dataPoint) throws SQLException {
        Connection conn = null;
        try {
            conn = BasicQueries.getDbConnection();
            if(conn == null) {
                System.out.println("conn: null");
            }
            String query = "INSERT INTO verbruik (huidigVerbruik, totVerbruikLaag, totVerbruikHoog, gasverbruik)" +
                    "values (?, ?, ?, ?);";
            PreparedStatement preparedStmt = conn.prepareStatement(query);
            preparedStmt.setDouble(1, dataPoint.getHuidigVerbruik());
            preparedStmt.setDouble(2, dataPoint.getTotVerbruikLaag());
            preparedStmt.setDouble(3, dataPoint.getTotVerbruikHoog());
            preparedStmt.setDouble(4, dataPoint.getGasverbruik());

            preparedStmt.execute();
            conn.close();
            System.out.println("Point written to database");
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            closeCon(conn);
        }
    }


    /**
     * Methode roept data per datapunt aan en propt het in een lijst
     * @return een ArrayList met meterData objecten, dit is alle data van de meter vertaald in objecten
     */
    private static ArrayList<meterData> getData() {
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
