import java.util.ArrayList;
import java.util.HashMap;

/**
 * Created by Hongzhilong on 16/1/4.
 */
public class _13211415_洪志龙_7_StrategyCatalog {
    private HashMap<_13211415_洪志龙_7_BookType, _13211415_洪志龙_7_IPricingStrategy> strategies;
    _13211415_洪志龙_7_StrategyCatalog(){
        strategies = new HashMap<_13211415_洪志龙_7_BookType,_13211415_洪志龙_7_IPricingStrategy>();
    }

    boolean hasStrategy(int strategyNumber){
        if(getKeyFromNumber(strategyNumber)== _13211415_洪志龙_7_BookType.NOTEXIST){
            return false;
        }
        else return true;
    }

    int size(){
        return strategies.size();
    }

    int getStrategyTypeFromNumber(int strategyNumber){
        _13211415_洪志龙_7_BookType bookType = getKeyFromNumber(strategyNumber);
        return strategies.get(bookType).getStrategyType();
    }

    _13211415_洪志龙_7_BookType getKeyFromNumber(int strategyNumber){
        for(HashMap.Entry<_13211415_洪志龙_7_BookType, _13211415_洪志龙_7_IPricingStrategy> entry: strategies.entrySet()){
            if(entry.getValue().getStrategyNumber() == strategyNumber){
                return entry.getKey();
            }
        }
        return _13211415_洪志龙_7_BookType.NOTEXIST;
    }

    ArrayList<_13211415_洪志龙_7_IPricingStrategy> findCompsiteContainSimple(int strategyNumber){
        _13211415_洪志龙_7_IPricingStrategy simpleStrategy = strategies.get(getKeyFromNumber(strategyNumber));
        ArrayList<_13211415_洪志龙_7_IPricingStrategy> list = new ArrayList<_13211415_洪志龙_7_IPricingStrategy>();
        for(HashMap.Entry<_13211415_洪志龙_7_BookType, _13211415_洪志龙_7_IPricingStrategy> entry: strategies.entrySet()){
            _13211415_洪志龙_7_IPricingStrategy strategy = entry.getValue();
            if(strategy.getStrategyType() == 2){
                _13211415_洪志龙_7_CompositeStrategy compositeStrategy = (_13211415_洪志龙_7_CompositeStrategy)strategy;
                if(compositeStrategy.strategies.contains(simpleStrategy)){
                    list.add(compositeStrategy);
                }
            }
        }
        return list;
    }

    HashMap<_13211415_洪志龙_7_BookType, _13211415_洪志龙_7_IPricingStrategy> getStrategies(){
        return strategies;
    }

}
