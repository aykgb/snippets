function Persion() {
    this.age= 0;
    var closure = '123'
    setInterVal(function growUP() {
        this.age++;
        console.log(closure)
    }, 1000);
}

var p = new Persion();

function PersonX() {
    'use strict'
    this.age= 0;
    var closure = '123'
    setInterVal(()=>{
        this.age++;
        console.log(closure)
    }, 1000)
}

var px = new PersonX();

