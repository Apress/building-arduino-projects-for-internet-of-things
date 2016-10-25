//
//  ViewController.swift
//  Smart Parking
//
//  Created by Adeel Javed on 11/6/15.
//  Copyright © 2015 CodifyThings. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var parkingSpots: UILabel!
        
    @IBAction func refreshParkingSpotsCount(sender: AnyObject) {
        
        let url = NSURL(string: "http://bookapps.codifythings.com/smartparking/getcount.php")
        let request = NSURLRequest(URL: url!)
        NSURLConnection.sendAsynchronousRequest(request, queue: NSOperationQueue.mainQueue()) {(response, data, error) in
            
            let jsonResponse: NSDictionary!=(try! NSJSONSerialization.JSONObjectWithData(data!, options: NSJSONReadingOptions.MutableContainers)) as! NSDictionary
                
            self.parkingSpots.text = "Open Parking Spots: " + String(jsonResponse["PARKING_SPOTS_COUNT"]!)
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

}`

