/**
 * Created by Hongzhilong on 16/1/4.
 */
public class _13211415_洪志龙_7_CompositeBestForCustomer extends _13211415_洪志龙_7_CompositeStrategy {

    _13211415_洪志龙_7_CompositeBestForCustomer(int strategyNumber){
        super(strategyNumber);
    }

    public double getSubTotal(_13211415_洪志龙_7_SaleLineItem saleLineItem) {
        double lowestMoney = Double.MAX_VALUE;

        for (_13211415_洪志龙_7_IPricingStrategy strategy : strategies) {
            double money = strategy.getSubTotal(saleLineItem);
            lowestMoney = Double.min(money, lowestMoney);
        }
        return lowestMoney;
    }
}
