/**
 * Created by Hongzhilong on 15/12/23.
 */
public class _13211415_洪志龙_7_PricingStrategyFactory {
    /* 策略工厂，工厂模式和单子模式 */
    private static _13211415_洪志龙_7_PricingStrategyFactory instance;
    private _13211415_洪志龙_7_StrategyCatalog catalog;

    private _13211415_洪志龙_7_PricingStrategyFactory(){
        //this.catalog=catalog;
    }

    public static _13211415_洪志龙_7_PricingStrategyFactory getInstance(){
        /* 单子模式，保证只有一个实例 */
        if(instance==null){
            instance = new _13211415_洪志龙_7_PricingStrategyFactory();
        }
        return instance;
    }

    public _13211415_洪志龙_7_IPricingStrategy getPricingStrategy(_13211415_洪志龙_7_BookType bookType){
        /* 根据书的类型返回不同策略 */
        _13211415_洪志龙_7_IPricingStrategy strategy = catalog.getStrategies().get(bookType);
        if(strategy!=null){
            return strategy;
        }
        else return new _13211415_洪志龙_7_NoDiscountStrategy();
    }
    public void setCatalog(_13211415_洪志龙_7_StrategyCatalog catalog){
        this.catalog=catalog;
    }
}
