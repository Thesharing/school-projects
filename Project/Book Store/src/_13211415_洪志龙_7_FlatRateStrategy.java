/**
 * Created by Hongzhilong on 15/12/23.
 */
public class _13211415_洪志龙_7_FlatRateStrategy implements _13211415_洪志龙_7_IPricingStrategy {
    private int discountPerBook;
    private int strategyNumber = 0;
    private int strategyType = 0;

    public _13211415_洪志龙_7_FlatRateStrategy(int strategyNumber, int discountPerBook) {
        this.strategyNumber = strategyNumber;
        this.discountPerBook = discountPerBook;
    }

    public int getStrategyNumber() {
        return strategyNumber;
    }

    public int getStrategyType() {
        return strategyType;
    }

    public void setStrategyNumber(int strategyNumber){
        this.strategyNumber=strategyNumber;
    }

    public void setDiscountPerBook(int discountPerBook){
        this.discountPerBook=discountPerBook;
    }

    public double getSubTotal(_13211415_洪志龙_7_SaleLineItem saleLineItem) {
        double total = saleLineItem.prodSpec.getPrice() - this.discountPerBook;
        if(total > 0)
            return total;
        else return 0;
    }

    @Override
    public String toString(){
        return "每本书减"+String.valueOf(discountPerBook)+"元";
    }
}
