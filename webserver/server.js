const http = require('http');
const spawn = require('child_process').spawn;


needed_ingredients = ["csirke", "spenot", "karfiol", "marhahus", "jegsalata", "sargarepa", "zoldborso", "fejessalata", "brokkoli", "fokhagyma", "csirkemaj", "kacsamaj", "tojas", "tej", "szardinia", "sajt", "tonhal"]

function run (cmd, cb) {
	var j_container = []
	for (let k = 0; k < needed_ingredients.length; k++)
	{
		var command = spawn(cmd, [needed_ingredients[k]]);
		var result = "";
		command.stdout.on('data', function(data) {
			result += data.toString();
		});
		command.on('close', function(code) {
			cb(result, j_container, k);
		});
	}

}

run("./search_engine", function(data_to_process, j_container, counter) {
	temp = data_to_process.split('\n');
	for (let i = 0; i < temp.length - 1; i++)
	{
		try {
			j_container.push(JSON.parse(temp[i].replaceAll("'", '"')));
		}
		catch {

		}
	}
	if (counter == 12)
	{
		setTimeout(function run_web_server () {
			var data = '<!DOCTYPE html> <html> <head> <meta charset="utf-8"> <meta name="viewport" content="width=device-width, initial-scale=1"> <title>Recipe Gatherer</title> <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.6.1/dist/css/bootstrap.min.css"> </head> <body> <div class="container"> <table class="table table-hover table-dark"> <thead> <tr> <th class="h3 font-weight-bold">Recipe Name</th> <th class="h3 font-weight-bold">Ingredients</th> <th class="h3 font-weight-bold">Recipe Url</th> </tr> </thead> <tbody>';
			for (let i = 0; i < j_container.length; i++)
			{
				try {
					data += '<tr><td class="p-3 h5">' + j_container[i].name.replaceAll('-', ' ').replace(j_container[i].name[0], j_container[i].name[0].toUpperCase()) + '</td><td>' + j_container[i].recipe_ingredients.toString().replaceAll(',', ', ') + '</td><td class="p-3"><a class="text-light" href="' + j_container[i].recipe_url + '">' + j_container[i].recipe_url + '</a> </td></tr>';
				}
				catch {
					
				}
			}
			data += '</tbody> </table> </div> </body> </html>';

			var server = http.createServer(function (req, res) {
				res.writeHead(200, {'Content-Type':'text/html'});
				res.write(data);
				res.end();
			});
			server.listen(5000);

			console.log('Node.js web server at address http://127.0.0.1:5000 is running..')
		}, 1000);
	}
});


/*
for (let k = 0; k < needed_ingredients.length; k++)
{
	let child = spawn("./search_engine", [needed_ingredients[k]])
	var temp = [];
	var j_container = [];
	child.stdout.on("data", (data) => {
		
		console.log(j_container);
	});
}
*/

/*
// console.log(j_container[0]);

*/
