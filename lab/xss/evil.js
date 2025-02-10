////////////////////////////////////////////////////////////////
const http = require( "http" );
const url = require( "url" );
const fs = require('fs');
const path = require('path');
////////////////////////////////////////////////////////////////
const host = '0.0.0.0';
const port = 8765;
////////////////////////////////////////////////////////////////
const router = {
    get: function( path, callback ){
        this[ path ] = callback;
    }
};
////////////////////////////////////////////////////////////////
http.createServer(( req, res ) => {
    const endpoint = url.parse( req.url, true );
    const path = endpoint.pathname;
    if( router[ path ]){
        req.query = endpoint.query;
        router[ path ]( req, res );
    } else {
        res.writeHead( 404, { "Content-Type": "text/plain" });
        res.end( "404 - What us Thus?\n" );
    }
}).listen( port, host, () => {
    console.log( `Listening on http://${host}:${port}` );
});
////////////////////////////////////////////////////////////////
router.get( "/muaHaHa", ( req, res ) => {
    console.log( req.query );
});
////////////////////////////////////////////////////////////////
router.get( "/", ( req, res ) => {
    const html = path.join( __dirname, "malicious.html" );
    fs.readFile( html, ( err, data ) => {
        if( err ){
            res.writeHead( 500, { 'Content-Type': 'text/plain' });
            res.end( 'Internal Server Error '); // :)
        } else {
            res.writeHead( 200, {
                'Content-Type': 'text/html',
                'Content-Length': data.length
            });
            res.end( data );
        }
    });
});
////////////////////////////////////////////////////////////////
////// <> 2.1.13. A system of three identical balls connected by
////// identical springs is suspended on a thread. The thread is
// burned. Find the accelerations of the balls immediately after
////////////////////////////////////////// the thread is burned.
// ############|################################################
// ###         |       #### Before burning the thread the system
// ### y      _O_      ##### is in a state of equilibrium, which
// ###       _\ /_     ###### is possible if the lower spring is 
// #####    _\   /_    ######### compressed and the side springs
// #####   _\     /_   ### are streched. Let say the deformation
// #####  _\       /_  ##### force of side springs is F, and the
// #####  \         /  ######## tension force is T. If masses of
// ##### O\/\/\/\/\/\O ####### the balls are m, ve can write the
// #####               ########## Newton laws in y direction as:
// ##### x ##################################################### 
// ########################## T = mg + 2(Fy), for the upper ball
// ##################### Fy = mg, for the side balls, zo T = 3mg
// #############################################################
// ###### Immediately after burrning the thread, the forces will
// ######## be the same except that T will become 0, so the side 
// ############ balls will have acceleration 0, and the upper 3g
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
