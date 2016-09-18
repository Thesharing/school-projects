/**
 * Created by Hongzhilong on 15/12/23.
 */
public class _13211415_洪志龙_7_NoDiscountStrategy implements _13211415_洪志龙_7_IPricingStrategy {
    public double getSubTotal(_13211415_洪志龙_7_SaleLineItem saleLineItem) {
        return saleLineItem.prodSpec.getPrice();
    }
    public int getStrategyNumber(){
        return 0;
    }
    public int getStrategyType(){
        return 3;
    }
    public String toString(){
        return "无策略";
    }
}
