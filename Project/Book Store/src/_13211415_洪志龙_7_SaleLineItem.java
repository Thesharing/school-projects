/**
 * Created by Hongzhilong on 15/12/21.
 */
public class _13211415_洪志龙_7_SaleLineItem {
    int copies;
    _13211415_洪志龙_7_BookSpecification prodSpec;
    _13211415_洪志龙_7_IPricingStrategy strategy;

    public _13211415_洪志龙_7_SaleLineItem(int copies, _13211415_洪志龙_7_BookSpecification prodSpec,
                                        _13211415_洪志龙_7_IPricingStrategy strategy){
        this.copies=copies;
        this.prodSpec=prodSpec;
        this.strategy=strategy;
    }


    public double getSubTotal(){
        return strategy.getSubTotal(this)*copies;
    }
}
