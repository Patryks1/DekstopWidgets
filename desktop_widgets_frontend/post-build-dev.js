var path = require('path');
var ncp = require('ncp').ncp;
var fs = require('fs');

ncp.limit = 16;

var srcPath = './build';
var destPath = '../build/debug/assets';
var assetsdestPath = '../assets';

var deleteFolderRecursive = function(path) {
  if( fs.existsSync(path) ) {
    fs.readdirSync(path).forEach(function(file,index){
      var curPath = path + "/" + file;
      if(fs.lstatSync(curPath).isDirectory()) {
        deleteFolderRecursive(curPath);
      } else {
        fs.unlinkSync(curPath);
      }
    });
    fs.rmdirSync(path);
  }
};

console.log('Deleting directory...');
deleteFolderRecursive(destPath);
console.log('Directory deleted');

console.log('Creating directory...');
if (!fs.existsSync(destPath)){
  fs.mkdirSync(destPath);
}
console.log('Directory created');

console.log('Copying files...');
ncp(srcPath, destPath, function (err) {
  if (err) {
    return console.error(err);
  }
  console.log('Copying files complete.');
});

console.log('Copying files to assets...');
ncp(srcPath, assetsdestPath, function (err) {
  if (err) {
    return console.error(err);
  }
  console.log('Copying files complete.');
});