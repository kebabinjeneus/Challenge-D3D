package API;

public class MeterData {
    private double huidigVerbruik;
    private double totVerbruikLaag;
    private double totVerbruikHoog;
    private double gasVerbruik;

    /**
     *
     * @param hv huidig verbruik stroom
     * @param tvl totaal verbruik stroom laag tarief
     * @param tvh totaal verbruik stroom hoog tarief
     * @param g gasverbruik
     */
    MeterData(double hv, double tvl, double tvh, double g) {
        huidigVerbruik = hv;
        totVerbruikLaag = tvl;
        totVerbruikHoog = tvh;
        gasVerbruik = g;
    }

    /**
     * @return huidig stroomverbruik
     */
    double getHuidigVerbruik() {
        return huidigVerbruik;
    }

    /**
     * @return totaal stroomverbruik laag tarief
     */
    double getTotVerbruikLaag() {
        return totVerbruikLaag;
    }

    /**
     * @return totaal stroomverbruik hoog tarief
     */
    double getTotVerbruikHoog() {
        return totVerbruikHoog;
    }

    /**
     * @return gasverbruik
     */
    double getGasVerbruik() {
        return gasVerbruik;
    }

    public String toString() {
        String out = String.format("Point read:\nhv: %.3f\ttvl: %.3f\ttvh: %.3f\tg: %.3f\n",
                huidigVerbruik, totVerbruikLaag, totVerbruikHoog, gasVerbruik);
        return out;
    }

}
