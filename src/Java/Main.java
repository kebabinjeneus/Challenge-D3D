import API.meterData;

import java.sql.SQLException;
import java.util.ArrayList;

public class Main {
    public static void main(String[] args) {
        public static void main(String[] args) throws SQLException {
            ArrayList<meterData> all = getData();
            System.out.println("The program will run for " + all.size()/6 + " minutes");
            for (meterData dataPoint : all) {
                jsonWriter(dataPoint);
                dbFiller(dataPoint);
                wait10();
            }
        }
    }
}
