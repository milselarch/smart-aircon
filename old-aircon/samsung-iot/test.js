const aircon = require('node-samsung-airconditioner');

aircon().on('discover', function(aircon) {
    // now login!
    aircon.get_token(function(err, token) {
        if (!!err) return console.log('login error: ' + err.message);
            // remember token for next time!
        }).on('waiting', function() {
            console.log('please power on the device within the next 30 seconds');
        }).on('end', function() {
            console.log('aircon disconnected');
        }).on('err', function(err) {
            console.log('aircon error: ' + err.message);
        });

        console.log(`POTATO ${aircon}`)
        aircon.login(token, function(err) {
            if (!!err) return console.log('login error: ' + err.message);
            // Drive the aircon!
            aircon.onoff(true);
        });
    }
).on('error', function(err) {
    console.log('discovery error: ' + err.message);
});
