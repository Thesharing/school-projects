/**
 * Created by Hongzhilong on 15/12/23.
 */
public class _13211415_洪志龙_7_PercentageStrategy implements _13211415_洪志龙_7_IPricingStrategy {
    private int strategyNumber;
    private int strategyType = 1;
    private double discountPercentage;

    public _13211415_洪志龙_7_PercentageStrategy(int strategyNumber, double discountPercentage) {
        this.strategyNumber = strategyNumber;
        this.discountPercentage = discountPercentage;
    }

    public int getStrategyNumber() {
        return this.strategyNumber;
    }

    public int getStrategyType() {
        return this.strategyType;
    }

    public void setStrategyNumber(int strategyNumber) {
        this.strategyNumber = strategyNumber;
    }

    public void setDiscountPercentage(double discountPercentage) {
        this.discountPercentage = discountPercentage;
    }

    public double getSubTotal(_13211415_洪志龙_7_SaleLineItem saleLineItem) {
        return saleLineItem.prodSpec.getPrice() * (1 - discountPercentage);
    }

    @Override
    public String toString() {
        return "打" + String.format("%.0f",100 * (1 - discountPercentage)) + "%折";
    }
}
