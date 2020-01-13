package Java;

import API.*;

import static API.mockReader.readIn;

public class Main {
    public static void main(String[] args) {
        meterData a = readIn(1);
        System.out.printf("hv: %f \tdvh: %f \tdvl: %f \tg: %f\n",
                a.getHuidigVerbruik(), a.getDagVerbruikHoog(),
                a.getDagVerbruikLaag(), a.getGasverbruik());
        meterData b = readIn(18387);
        System.out.printf("hv: %f \tdvh: %f \tdvl: %f \tg: %f\n",
                b.getHuidigVerbruik(), b.getDagVerbruikHoog(),
                b.getDagVerbruikLaag(), b.getGasverbruik());
    }
}
