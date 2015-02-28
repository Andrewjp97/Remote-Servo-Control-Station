//
//  AppDelegate.swift
//  Servo Control Station
//
//  Created by Andrew Paterson on 2/28/15.
//  Copyright (c) 2015 Andrew Paterson. All rights reserved.
//

import Cocoa
import CoreFoundation



@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {

    let URLString = "arduino.local"
    let port = 8888
    var socket: GCDAsyncUdpSocket?
    
    func setupConnection(){
        var error : NSError?
        socket = GCDAsyncUdpSocket(delegate: self, delegateQueue: dispatch_get_main_queue())
        if socket != nil {
            socket!.connectToHost(URLString, onPort: UInt16(port), error: &error)
            socket!.beginReceiving(&error)
            send("ping")
        }
        else if error != nil{
            let errorNum = 1
        }
    }
    
    func send(message:String){
        let data = message.dataUsingEncoding(NSUTF8StringEncoding)
        NSLog("Data Sent: ")
        NSLog(message)
        NSLog(String(UTF8String: message.cStringUsingEncoding(NSUTF8StringEncoding)!)!)
        socket?.sendData(data, withTimeout: 2, tag: 0)
    }
    
    @IBOutlet weak var window: NSWindow!


    func applicationDidFinishLaunching(aNotification: NSNotification) {
        // Insert code here to initialize your application
        setupConnection()
    }

    func applicationWillTerminate(aNotification: NSNotification) {
        // Insert code here to tear down your application
    }

    @IBOutlet weak var label: NSTextField!
    @IBAction func ValueChanged(sender: NSSlider) {
        let value = sender.intValue
        label.stringValue = String(value)
        NSLog("Label Value: ")
        NSLog(String(value))
        if label.stringValue.toInt() >= 10 && label.stringValue.toInt() < 100 {
            send("0" + label.stringValue)
        }
        else if label.stringValue.toInt() >= 100 {
            send(label.stringValue)
        }
        else {
            send("00" + label.stringValue)
        }
    }

}

