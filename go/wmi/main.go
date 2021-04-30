package main

import (
	"log"

	"github.com/go-ole/go-ole"
	"github.com/go-ole/go-ole/oleutil"
)


func main() {
	ole.CoInitialize(0)
    defer ole.CoUninitialize()

    unknown, err := oleutil.CreateObject("WbemScripting.SWbemLocator")
	if err != nil {
		log.Fatal(err)
	}
    defer unknown.Release()

    wmi, err := unknown.QueryInterface(ole.IID_IDispatch)
	if err != nil {
		log.Fatal(err)
	}
    defer wmi.Release()

    // service is a SWbemServices
    serviceRaw,err := oleutil.CallMethod(wmi, "ConnectServer")
	if err != nil {
		log.Fatal(err)
	}
    service := serviceRaw.ToIDispatch()
    defer service.Release()

    // result is a SWBemObjectSet
    resultRaw, err := oleutil.CallMethod(service, "ExecQuery", "select * from MSAcpi_ThermalZoneTemperature")
	if err != nil {
		log.Fatal(err)
	}
    result := resultRaw.ToIDispatch()
    defer result.Release()

    countVar, err := oleutil.GetProperty(result, "Count")
	if err != nil {
		log.Fatal(err)
	}
    count := int(countVar.Val)

    for i :=0; i < count; i++ {
        // item is a SWbemObject, but really a Win32_Process
        itemRaw, _ := oleutil.CallMethod(result, "ItemIndex", i)
        item := itemRaw.ToIDispatch()
        defer item.Release()

        asString, err := oleutil.GetProperty(item, "Name")
		if err != nil {
			log.Fatal(err)
		}
        println(asString.ToString())
    }
}
 