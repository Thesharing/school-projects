import java.util.ArrayList;

/**
 * Created by Hongzhilong on 16/1/4.
 */
public abstract class _13211415_洪志龙_7_CompositeStrategy implements _13211415_洪志龙_7_IPricingStrategy {
    private int strategyNumber;
    private int strategyType = 2;
    protected ArrayList<_13211415_洪志龙_7_IPricingStrategy> strategies;

    _13211415_洪志龙_7_CompositeStrategy(int strategyNumber){
        this.strategyNumber = strategyNumber;
        strategies = new ArrayList<_13211415_洪志龙_7_IPricingStrategy>();
    }

    public int getStrategyNumber() {
        return strategyNumber;
    }

    void add(_13211415_洪志龙_7_IPricingStrategy strategy) {
        strategies.add(strategy);
    }

    void remove(_13211415_洪志龙_7_IPricingStrategy strategy) {
        if (strategies.contains(strategy)) {
            strategies.remove(strategy);
        }
    }

    public int getStrategyType(){
        return this.strategyType;
    }

    public String toString(){
        String contain = "";
        for (_13211415_洪志龙_7_IPricingStrategy strategy : strategies) {
            contain+=String.valueOf(strategy.getStrategyNumber())+" ";
        }
        return "包含"+contain+"策略";
    }

    public boolean isEmpty(){
        return strategies.isEmpty();
    }

    /*boolean hasStrategy(_13211415_洪志龙_7_IPricingStrategy strategy) {
        return strategies.contains(strategy);
    }*/

    public abstract double getSubTotal(_13211415_洪志龙_7_SaleLineItem saleLineItem);

}
