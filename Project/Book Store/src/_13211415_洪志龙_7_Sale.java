import java.util.ArrayList;

/**
 * Created by Hongzhilong on 15/12/23.
 */
public class _13211415_洪志龙_7_Sale implements _13211415_洪志龙_7_Subject{
    private ArrayList<_13211415_洪志龙_7_SaleLineItem> items;
    private ArrayList<_13211415_洪志龙_7_Observer> observers;
    public _13211415_洪志龙_7_Sale(){
        items = new ArrayList<_13211415_洪志龙_7_SaleLineItem>();
        observers = new ArrayList<>();
    }

    void addSaleLineItem(_13211415_洪志龙_7_SaleLineItem item){
        items.add(item);
    }

    double getTotal(){
        double total = 0;
        double thisTime = 0;
        for (_13211415_洪志龙_7_SaleLineItem saleLineItem : items) {
            thisTime = saleLineItem.getSubTotal();
            total += thisTime;
            System.out.println(saleLineItem.prodSpec.getTitle());
            System.out.println(String.format("%.2f", thisTime));
        }
        return total;
    }

    public ArrayList<_13211415_洪志龙_7_SaleLineItem> getItems(){
        return items;
    }

    @Override
    public void registerObserver(_13211415_洪志龙_7_Observer observer) {
        observers.add(observer);
    }

    @Override
    public void removeObserver(_13211415_洪志龙_7_Observer observer) {
        if(observers.contains(observer)){
            observers.remove(observer);
        }
    }

    @Override
    public void notifyObservers() {
        for(_13211415_洪志龙_7_Observer observer:observers){
            observer.update(this);
        }
    }
}
