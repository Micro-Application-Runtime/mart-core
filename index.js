setTimeout(() => { console.log("1s timeout") }, 1000);

let myPromise = new Promise((resolve, reject) => {
    setTimeout(() => {
        console.log("promise")
        resolve("promise success");
    }, 3000);
});

myPromise.then((value) => {
    console.log(value);
})

console.log("end")