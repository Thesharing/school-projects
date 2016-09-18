import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

/**
 * Created by Hongzhilong on 16/1/4.
 */
public class _13211415_洪志龙_7_BuyUI extends JFrame implements ActionListener {
    private JPanel mainPanel = new JPanel();
    private JLabel labelBookTitle = new JLabel();
    private JTextField textFieldCount = new JTextField();
    private JButton buttonOk = new JButton("确定");
    private JButton buttonCancel = new JButton("取消");
    private _13211415_洪志龙_7_BookSpecification bookSpecification;
    private _13211415_洪志龙_7_Controller controller;

    _13211415_洪志龙_7_BuyUI(_13211415_洪志龙_7_Controller controller, String isdn){
        this.controller=controller;
        bookSpecification=controller.getBookSpecification(isdn);

        setTitle("购买书籍");
        GridLayout grid=new GridLayout(3,2);
        mainPanel.setLayout(grid);
        mainPanel.add(new JLabel("书名："));
        labelBookTitle.setText(bookSpecification.getTitle());
        mainPanel.add(labelBookTitle);
        mainPanel.add(new JLabel("数量："));
        textFieldCount.addKeyListener(new KeyAdapter(){
            public void keyTyped(KeyEvent e) {
                int keyChar = e.getKeyChar();
                if(keyChar >= KeyEvent.VK_0 && keyChar <= KeyEvent.VK_9){
                }
                else{
                    e.consume();
                }
            }
        });
        mainPanel.add(textFieldCount);
        mainPanel.add(buttonOk);
        mainPanel.add(buttonCancel);

        buttonOk.addActionListener(this);
        buttonCancel.addActionListener(this);

        add(mainPanel);
        pack();
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        int screenHeight = java.awt.Toolkit.getDefaultToolkit().getScreenSize().height;
        int screenWidth = java.awt.Toolkit.getDefaultToolkit().getScreenSize().width;
        setResizable(false);
        setSize(300, 100);
        setLocation((screenWidth - 300) / 2, (screenHeight - 100) / 2);
        setVisible(true);
    }

    public void actionPerformed(ActionEvent e) {
        if(e.getSource()==buttonOk){
            int copies = 0;
            if(textFieldCount.getText().isEmpty()){
                JOptionPane.showMessageDialog(null,"未填写数量。","错误",JOptionPane.ERROR_MESSAGE);
                return;
            }
            else{
                try{
                    copies = Integer.parseInt(textFieldCount.getText());
                }
                catch (java.lang.NumberFormatException e1){
                    JOptionPane.showMessageDialog(null,"数量填写格式错误。","错误",JOptionPane.ERROR_MESSAGE);
                    return;
                }
                if(controller.buyBook(copies,bookSpecification.getIsbn())){
                    JOptionPane.showMessageDialog(null,"已加入购物车。","成功",JOptionPane.INFORMATION_MESSAGE);
                    setVisible(false);
                    return;
                }
                else{
                    JOptionPane.showMessageDialog(null,"加入购物车时出现问题，请重试。","错误",JOptionPane.ERROR_MESSAGE);
                    return;
                }
            }
        }
        else if(e.getSource()==buttonCancel){
            setVisible(false);
            return;
        }
    }
}
