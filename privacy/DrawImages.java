import org.eclipse.swt.SWT;
import org.eclipse.swt.events.PaintEvent;
import org.eclipse.swt.events.PaintListener;
import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.graphics.Image;
//import org.eclipse.swt.graphics.ImageData;
//import org.eclipse.swt.graphics.ImageLoader;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Canvas;
//import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;
import java.io.*;
import java.net.*;

/**
 * 
 */

public class DrawImages {
  Display display = new Display();
  Shell shell = new Shell(display);
	private int testbed_dim_x=345; //uzl_x
	private int testbed_dim_y=140; //uzl_y
	private int target_size_percent=23;
	static	String color_list[][]= new String [256][4];
	private int list_card = 0;
	private boolean flag = false;
	
//inizializzazione dell'oggetto ServerSocket
  
  public DrawImages(Socket socket) throws InterruptedException, IOException {
    shell.setLayout(new FillLayout());
    Canvas canvas = new Canvas(shell, SWT.NULL);
    
    final Image image = new Image(display, "/home/antonio/Scrivania/uzl_testbed.png");
	final Image scaled_image = new Image(display, image.getImageData().scaledTo(800, 325) );
    GC gc = new GC(scaled_image);      
    
    gc.setBackground(display.getSystemColor(SWT.COLOR_BLUE ) );
   // gc.fillOval(1, 1, 45, 45);
    canvas.addPaintListener(new PaintListener() {
      public void paintControl(PaintEvent e) {
        e.gc.drawImage(scaled_image, 0, 0);
       // e.gc.drawImage(image, 0, 0, 100, 100, 200, 10, 200, 50);
      }
    });
    shell.setSize(800, 325);
    shell.open();
    //textUser.forceFocus();
    //captureControl(canvas, "canvas.bmp");
    //do what you want to do before sleeping
    //Thread.currentThread().sleep(3000);//sleep for 1000 ms
    //do what you want to do after sleeptig
    // Set up the event loop.
    while (!shell.isDisposed()) {
      if (!display.readAndDispatch()) {
        // If no more entries in event queue
 
    	  
    	//  display.sleep();
        System.out.println("prova");
        //Thread.currentThread().sleep(1000);
       String result  = get_coord(socket);
         
       String vis = result.substring(0, 3);
       System.out.println("ricevuto   : "+result);

       int pos,pos2,pos3,pos4;
       pos = result.indexOf("-"); 
       pos2 = result.indexOf("-", pos+1);
       pos3 = result.indexOf("-", pos2+1);
       pos4 = result.indexOf("-", pos3+1);
       System.out.println("ricevuto   : "+result+" "+pos+" "+ pos2+ " " +pos3+ " "+pos4 );
       int find_id,color=0;
       String id = result.substring(pos+1,pos2);
       String x = result.substring(pos2+1,pos3);
       String y = result.substring(pos3+1,pos4);
       
       flag = false;
       
       for(find_id=0;find_id<list_card;find_id++)
       {	
    	   System.out.println(color_list[find_id][0]+" "+id);
    	   if (id.equals(color_list[find_id][0]))
    	   {	System.out.println(" target old =" +id + " colore " + color_list[find_id][1] );

    		   color=Integer.parseInt(color_list[find_id][1]);
    		   color_list[find_id][2]=""+x;
    		   color_list[find_id][3]=""+y;
    		   flag = true;
    		}
       }
       
       if (flag==false)
       {
    	   color_list[list_card][0]= id;
    	   	color_list[list_card][1]=""+(list_card+3);
    	   	list_card++;
    		   color_list[find_id][2]=""+x;
    		   color_list[find_id][3]=""+y;
    	   color = list_card+3;
    	   System.out.println("Nuovo target =" +id + " nuovo colore "+ list_card);
       }
       
       
 
          final int target_x = Integer.parseInt(x);
          final int target_y= Integer.parseInt(y);
          final int color2 = color; 
         //int h= intArrayOutput[2];
         System.out.println("coordinate  : "+target_x +" "+ target_y+" "+result);

          int disp_x = 0;
          int disp_y = 0;
         
         
         if ( target_x >  testbed_dim_x - (800 + 325) /2*target_size_percent/100/2 )
		  {
		     disp_x = (800 + 325)/2*target_size_percent/100;
		  }
		  if (target_y > testbed_dim_y - (800 + 325)/2*target_size_percent/100/2 )
		  {
		     disp_y = (800 + 325)/2*target_size_percent/100;
		  }
		  
		 
		  canvas.addPaintListener(new PaintListener() {
		      public void paintControl(PaintEvent e) {
		    	  		        e.gc.drawImage(scaled_image, 0, 0);
		    	  		        
		        e.gc.setBackground(display.getSystemColor(color2 ) );
		        int x=target_x,y=target_y;
		     int count;   	
	    	  e.gc.setAlpha( 90 );

		     for(count=0;count<list_card;count++){
			        e.gc.setBackground(display.getSystemColor(Integer.parseInt(color_list[count][1]) ) );

		    	 x=Integer.parseInt(color_list[count][2]);
			     if(x>=330) x=300;

		    	 y=Integer.parseInt(color_list[count][3]);
			     if(y>(testbed_dim_y/2))
			    	 y = y-(testbed_dim_y-y)/2;
			     else 
			    	 y = y+(testbed_dim_y-y)/2;

			        e.gc.fillOval(800*x/testbed_dim_x - 0, (325*y/testbed_dim_y )-80, (800 + 325)/2*(target_size_percent-(count+1)*2)/100 , (800 + 325)/2*(target_size_percent-(count+1)*2)/100 );
		     }
		   
		     }
		    });
		   
		  
         //gc.fillOval(900*target_x/testbed_dim_x - disp_x, 700*target_y/testbed_dim_y - disp_y, (900 + 700)/2*target_size_percent/100 , (900 + 700)/2*target_size_percent/100 );
         
         
         // gc.fillOval(z, x , h, 55);        
         //System.out.println(z+" "+x+" "+h);
         //display.update();        
        canvas.redraw();
      }      
    }

    display.dispose();
  }
  /**
   * Captures the specified control and saves the result into a file in the BMP format.
   * @param control
   * @param fileName
 * @throws IOException 
   */
 String get_coord(Socket socket)   throws IOException{
             
	     //apro il canale di comunicazione con il client
	     //DataInputStream is = new DataInputStream(socket.getInputStream());  
	     BufferedReader is = new BufferedReader(new InputStreamReader(socket.getInputStream()));
	     //leggo i dati scritti, l'uscita viene dettata dalla stringa QUIT

	     System.out.println("Before readline");
	     String res = is.readLine();
	     System.out.println("After readline");

	     //is.close();
	     //socket.close();
	     System.out.println("Il client è stato soddisfatto...il server torna a dormire...");
	  return res;
	   
  }

 // private void init() {  }

  public static void main(String[] args) throws InterruptedException , IOException
{  int i,y;
      System.out.println("Colori : "+ SWT.COLOR_BLUE + " " + SWT.COLOR_BLACK + " " + SWT.COLOR_GRAY + " "+ SWT.COLOR_CYAN);
  	for(i=0;i<256;i++){
		color_list[i][0]="";
		color_list[i][1]="";
		}
	  
	  ServerSocket serverFather = new ServerSocket(1745);
	  //recupero info su server

	   InetAddress info = serverFather.getInetAddress();
	   String server = info.getHostAddress();
	   int port = serverFather.getLocalPort();
	    
	   System.out.println("Nome:"+ server + " Porta:"+ port);
	   //ciclo infinito
	
	     System.out.println("Il server va in sleep...");
	     //accesso le richieste del client
	     Socket socket = serverFather.accept();
	     System.out.println("Il server si sveglia...");
	     //trovo le info sul client
	     InetAddress infoclient = socket.getInetAddress();  
	     String client = infoclient.getHostAddress();       
	     int portclient = socket.getLocalPort();
	     System.out.println("Il client " + client + " " + portclient +  " ha svegliato il server...");
	   
    new DrawImages(socket);
  
}}