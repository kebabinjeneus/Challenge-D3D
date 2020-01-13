package API;

public class meterData {
    private double huidigVerbruik;
    private double totVerbruikLaag;
    private double totVerbruikHoog;
    private double gasverbruik;

    public meterData(double hv, double tvl, double tvh, double g) {
        huidigVerbruik = hv;
        totVerbruikLaag = tvl;
        totVerbruikHoog = tvh;
        gasverbruik = g;
    }

    public double getHuidigVerbruik() {
        return huidigVerbruik;
    }

    public double getDagVerbruikLaag() {
        return totVerbruikLaag;
    }

    public double getDagVerbruikHoog() {
        return totVerbruikHoog;
    }

    public double getGasverbruik() {
        return gasverbruik;
    }

}
