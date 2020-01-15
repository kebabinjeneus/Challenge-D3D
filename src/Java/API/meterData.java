package API;

public class meterData {
    private double huidigVerbruik;
    private double totVerbruikLaag;
    private double totVerbruikHoog;
    private double gasverbruik;

    /**
     *
     * @param hv huidig verbruik stroom
     * @param tvl totaal verbruik stroom laag tarief
     * @param tvh totaal verbruik stroom hoog tarief
     * @param g gasverbruik
     */
    public meterData(double hv, double tvl, double tvh, double g) {
        huidigVerbruik = hv;
        totVerbruikLaag = tvl;
        totVerbruikHoog = tvh;
        gasverbruik = g;
    }

    /**
     * @return huidig stroomverbruik
     */
    public double getHuidigVerbruik() {
        return huidigVerbruik;
    }

    /**
     * @return totaal stroomverbruik laag tarief
     */
    public double getDagVerbruikLaag() {
        return totVerbruikLaag;
    }

    /**
     * @return totaal stroomverbruik hoog tarief
     */
    public double getDagVerbruikHoog() {
        return totVerbruikHoog;
    }

    /**
     * @return gasverbruik
     */
    public double getGasverbruik() {
        return gasverbruik;
    }

}
