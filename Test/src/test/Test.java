/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package test;
import java.util.ArrayList;
import net.java.games.input.Component;
import net.java.games.input.Component.Identifier;
import net.java.games.input.Controller;
import net.java.games.input.ControllerEnvironment;
import arduino.Arduino;
import java.util.Scanner;
/**
 *
 * @author Goblin
 */
class Wait {
  public static void bySeconds(long s) {
    try {
      Thread.currentThread().sleep(s * 1000);
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
  }
}
//        Scanner scanner = new Scanner(System.in);
//        Arduino arduino = new Arduino("COM6", 9600);
//
//        boolean connected = arduino.openConnection();
//        System.out.println("Соединение установлено: " + connected);
//        Thread.sleep(2000);
//        label_1:
//        while (scanner.hasNext()) {
//
//            String s = scanner.nextLine();
//            arduino.serialWrite(s);
//        }
public class Test 
{
    Scanner scanner = new Scanner(System.in);
    Arduino arduino = new Arduino("COM3", 19200);
    boolean connected = arduino.openConnection();     
    private ArrayList<Controller> foundControllers;
    public static void main(String[] args) 
    {
            new Test().start();
    }    
    void start() 
    {
        foundControllers = new ArrayList<>();
        searchForControllers();
        //Если был обнаружен хотя бы один контроллер, 
        //мы начнем показывать данные контроллера в окне.
        if(!foundControllers.isEmpty())
            startShowingControllerData();
        else
            System.out.println("No controller found!");
    }
    void searchForControllers() 
    {
        Controller[] controllers = ControllerEnvironment.getDefaultEnvironment().getControllers();

        for(int i = 0; i < controllers.length; i++){
            Controller controller = controllers[i];
            
            if (
                    controller.getType() == Controller.Type.STICK || 
                    controller.getType() == Controller.Type.GAMEPAD || 
                    controller.getType() == Controller.Type.WHEEL ||
                    controller.getType() == Controller.Type.FINGERSTICK
               )
            {
                // Add new controller to the list of all controllers.
                // Добавление нового контроллера в список всех контроллеров.
                foundControllers.add(controller);
                
                // Add new controller to the list on the window.
                // Добавить новый контроллер в список в окне.
                System.out.println(controller.getName() + " - " + controller.getType().toString() + " type");
            }
        }
    }
    void startShowingControllerData()
    {
        while(true)
        {
            int selectedControllerIndex = 0;
            Controller controller = foundControllers.get(selectedControllerIndex);
            if( !controller.poll() )
            {
                break;
            }
            // X axis and Y axis
            int xAxisPercentage = 0;
            int yAxisPercentage = 0;
            Component[] components = controller.getComponents();
            for(int i=0; i < components.length; i++)
            {   
                Component component = components[i];
                Identifier componentIdentifier = component.getIdentifier();
                
                // Buttons
                //if(component.getName().contains("Button")){ // If the language is not english, this won't work.
                if(componentIdentifier.getName().matches("^[0-9]*$")){ 
                    // If the component identifier name contains only numbers, then this is a button.
                    // Если имя идентификатора компонента содержит только числа, то это кнопка.
                    // Is button pressed?
                    // Нажимается ли кнопка?
                    boolean isItPressed = true;
                    if(component.getPollData() == 0.0f)
                        isItPressed = false;
                    
                    // Button index
                    String buttonIndex;
                    buttonIndex = component.getIdentifier().toString();
                    // We know that this component was button so we can skip to next component.
                    continue;
                }
                
                // Hat switch
                // 
                if(componentIdentifier == Component.Identifier.Axis.POV){
                    float hatSwitchPosition = component.getPollData();
                    
                    // We know that this component was hat switch so
                    //we can skip to next component.
                    // Мы знаем, что этот компонент был переключателем шляпы, 
                    //поэтому мы можем перейти к следующему компоненту.
                    continue;
                }
                
                // Axes
                if(component.isAnalog()){
                    float axisValue = component.getPollData();
                    int axisValueInPercentage = getAxisValueInPercentage(axisValue);
                    
                    // X axis
                    if(componentIdentifier == Component.Identifier.Axis.X){
                        xAxisPercentage = axisValueInPercentage;                        
                        continue; // Go to next component.
                    }
                    // Y axis
                    if(componentIdentifier == Component.Identifier.Axis.Y){
                        yAxisPercentage = axisValueInPercentage;
                        continue; // Go to next component.
                    }
                }
            String x = String.format("%03d", xAxisPercentage);
            String y = String.format("%03d", yAxisPercentage);
            //xAxisPercentage =   String.format("%08d",xAxisPercentage);
            String  s   =     ("x"+x+"y"+y);
//      x и y поменяй местами!!!!!!!
//            !!!!!
//            !!!!!
//            !!!!!
//            !!!!!
//            !!!!!
//            !!!!!
//            !!!!!
//            !!!!!
//            !!!!!
//            !!!!!

//            !!!!!
//            !!!!!
            //String f = "x100y050";
            arduino.serialWrite(s);
            //String  d   = arduino.serialRead();
            System.out.println(s);
            //System.out.println(d);
            System .out.println(x);
            System.out.println(y);
            Wait.bySeconds(1);
            }
        }
     }
    public int getAxisValueInPercentage(float axisValue)
    {
        return (int)(((2 - (1 - axisValue)) * 100) / 2);
    }
}

    

