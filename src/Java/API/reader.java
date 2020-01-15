package API;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class reader {


    /**
     * Leest de data in vanuit het bestand p1.data
     *
     * @param startLine regel waar het inlezen start
     * @return een meterData object met het datapunt dat start op de regel startLine
     */
    static meterData readIn(int startLine) {
        File p1 = new File(
                "C:\\Users\\Lars\\Documents\\Challenge-D3D\\src\\resources\\p1.data"
        ); // TODO pad relatief maken en rootpad aan systeem vragen
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

}