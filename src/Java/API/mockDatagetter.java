package API;

import com.google.gson.Gson;

import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

import static API.mockReader.readIn;

public class mockDatagetter {
    public static void main(String[] args) {
        Gson gson = new Gson();
        ArrayList<meterData> all = getData();
        for (meterData dataPoint : all) {
            try {
                String jsonString = gson.toJson(dataPoint);
                FileWriter writer = new FileWriter(
                        "C:\\Users\\Lars\\Documents\\Challenge-D3D\\src\\resources\\currentDataPoint.json"
                );
                writer.write(jsonString);
                writer.close();
                System.out.println("written next point");
            } catch (IOException e) {
                System.out.println("exception " + e.getMessage());
                e.printStackTrace();
            }
            wait10();
        }
    }

    private static ArrayList<meterData> getData() {
        ArrayList<meterData> out = new ArrayList<meterData>();
        int line = 31; // 57 83
        for(int i = 0; true; i++) {
            out.add(readIn(line));
            line += 26;
            if(line > 18400) {
                break;
            }
        }
        return out;
    }

    private static void wait10() {
        try {
            Thread.sleep(9990);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
