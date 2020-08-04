load("/etc/intbee/mongo.js");
var opensku=connectdb("opensku")
load("id.js")
print("start test")
for(var i =0; i<data.length; i++){
    let cps=data[i]
    print(cps)
    let task_info = opensku.opensku_product_stock.find({"product_id":data[1]})
    parseFloat()
    if(task_info && task_info.product_price){
        let rate = cps[3]/task_info.product_price * 1000
        if(rate == cps[2]){
            print("task[" + cps[0] + "] ok")
        }else
        {
            print("task[" + cps[0] + "] error: "+ rate)
        }
    }else {
        print("task[" + cps[0] + "] not found")
    }

}
print("finished test")