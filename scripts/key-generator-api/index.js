var http = require('http')
var ecc = require('./seatjs-ecc')


var srv = http.createServer(function (req, res) {
  let keys = {
    privateKey: '',
    publicKey: ''
  }
  res.writeHead(200, {'Content-Type': 'application/json'})
  if( req.url === '/api/key-generator') {
    ecc.randomKey()
      .then(privateKey => {
        keys.privateKey = privateKey.toString()
        keys.publicKey = 'SEAT' + ecc.privateToPublic(keys.privateKey).slice(3)
        res.end(JSON.stringify(keys));
      })
  }
  
})


srv.listen(8888, function() {
  console.log('listening on 8888');
})