package API;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class reader {


    /**
     * Leest de data in vanuit het bestand p1.data
     *
     * @param startLine regel waar het inlezen start
     * @return een meterData object met het datapunt dat start op de regel startLine
     */
    private static meterData readIn(int startLine) {
        File p1 = new File(
                System.getProperty("user.dir") + "\\resources\\p1.data"
                // TODO change to proper location on pi -> /home/p1.data <- oid
        );
        Scanner file;
        try {
            file = new Scanner(p1);
        } catch (FileNotFoundException a) {
            a.printStackTrace();
            return null;
        }
        for (int i = 1; i < startLine; i++) {
            file.nextLine();
        }

        double hv = 0, tvl = 0, tvh = 0, g = 0;
        while (file.hasNextLine()) {
            String line = file.nextLine();
            if (!line.matches("#(.*)") && !line.matches("/(.*)")) {
                if (line.matches("!(.*)")) {
                    break;
                } else {
                    if (line.matches("1-0:1.8.1(.*)")) {
                        tvl = Double.parseDouble(line.substring(11, 20));
                    } else if (line.matches("1-0:1.8.2(.*)")) {
                        tvh = Double.parseDouble(line.substring(11, 20));
                    } else if (line.matches("1-0:1.7.0(.*)")) {
                        hv = Double.parseDouble(line.substring(11, 16));
                    } else if (line.matches("0-1:24.2.1(.*)")) {
                        g = Double.parseDouble(line.substring(27, 35));
                    }
                }
            }
        }
        return new meterData(hv, tvl, tvh, g);
    }

    /**
     * Methode roept data per datapunt aan en propt het in een lijst
     * @return een ArrayList met meterData objecten, dit is alle data van de meter vertaald in objecten
     */
    public static ArrayList<meterData> getAllData() {
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

    public reader() {

    }
    private int currentLine = 31;

    public meterData getNextDatapoint() {
        meterData Datapoint = readIn(currentLine);
        lineUpper();
        return Datapoint;
    }

    public int getCurrentLine() { return currentLine; }
    private void lineUpper() { currentLine+=26; }
}