import urllib.request
import time
import json
from sys import stdout

begin = time.time()

def clarify(r, split_char):
    # print ("Running \"clarify\"...")
    try:
        response = str(r.read())
    except Exception as e:
        print ("Something lost...")
        while (checker != 1):
            checker = 0
            try:
                clarify(r, split_char)
            except:
                print ("Something still is lost...")
                checker = 0
            else:
                checker = 1


    resp = response.strip().replace("\\n", "").split(split_char) # Splitting response text into more readable array
    remade = []
    for o in resp:
        remade.append(o.strip()) # Remove any unwanted spaces after string
    return remade

class GetRecipeSites:
    def __init__(self):
        self.page_id = 1
        self.recipes_urls = []
        self.base_url = "https://www.nosalty.hu/receptek?page="
        self.page_count = 3 # page_count = 476

    def get_urls(self, r):
        remade = clarify(r, '<')
        possible_urls = []
        for i in remade:
            if ("/recept/" in i): # Check if wanted phrase is in url
                possible_urls.append(i)

        for possible in possible_urls:
            if ("class=\"position-relative d-block\"" in possible):
                temp = possible.split(' ')
                self.recipes_urls.append("https://www.nosalty.hu" + temp[3].split('"')[1])

    def run(self):
        while (self.page_id <= self.page_count):
            r = urllib.request.urlopen(f"{self.base_url}{str(self.page_id)}")
            self.get_urls(r)
            print ("\b"*750 + f"Running for {round(time.time() - begin, 1)}s: Page ID: {self.page_id} out of {self.page_count}", end="")
            stdout.flush()
            
            self.page_id += 1

    def display(self):
        for url in self.recipes_urls:
            print(url)

    def get_list_of_urls(self):
        return self.recipes_urls

class GetRecipeData:
    def __init__(self, recipes_urls):
        self.recipes_urls = recipes_urls
        self.recipe_datas = []
        self.recipe_data = {
            "name":"",
            "url":"",
            "ingredients":[]
        }
        self.recipes = []
    def get_ingredients(self, url, urls_length, i):
        # print ("Running \"get_ingredients\"...")
        self.recipe_data["name"] = ""
        self.recipe_data["url"] = ""
        self.recipe_data["ingredients"] = []
        u = url.split('/')
        self.recipe_data["name"] = u[4]
        self.recipe_data["url"] = url
        print ("\b"*1000 + f"Running for {round(time.time() - begin, 1)}s: Urls: {i + 1} out of {urls_length} DONE!", end="")
        time.sleep(5)
        stdout.flush()
        r = urllib.request.urlopen(f"{url}")

        remade = clarify(r, '<li')

        for k in remade:
            if ("m-list__item p-2 -dotted -fontSize-16 d-flex justify-content-between pl-5" in k):
                temp = k.split(' ') # Splitting with spaces to separate href part of the response
                for i in temp:
                    if ("href" in i):
                        temp2 = i.split('/') # Splitting separated links to get ingredients
                        if (temp2[1] == "alapanyag"):
                            if (temp2[2][:-2] not in self.recipe_data["ingredients"]): # Checking if ingredient is already added to list
                                self.recipe_data["ingredients"].append(temp2[2][:-2]) # Append ingredients to recipe_data object by cutting the last three characters down
        
        return self.recipe_data

    def run(self):
        # Reading saved ingredients list into data
        d = open("temp_db", "r")
        data = d.read()
        d.close()
        # Reading saved url_index
        r = open("url_index", "r")
        i = int(r.read())
        r.close()
        # Dealing with url_index so it won't skip the for loop
        if (i == len(self.recipes_urls)):
            # Resetting url_index to 0
            w = open("url_index", "w")
            w.write("0")
            w.flush()
            w.close()
            # Clearing last temp_db content
            w = open("temp_db", "w")
            w.write("")
            w.flush()
            w.close()
        
        recipes_urls = sorted(self.recipes_urls, key=len)

        u = open("url_index", "w")
        t = open("temp_db", "a")

        for k in range(len(recipes_urls) - i):
            data += str(self.get_ingredients(recipes_urls[i], len(recipes_urls), i)) + ';'
            i += 1
            # Saving current url_index to url_index file
            u.write(str(i))
            u.flush()
            # Appending data to temp_db
            t.write(str(self.get_ingredients(recipes_urls[i], len(recipes_urls), i)) + ";")
            t.flush()
            time.sleep(1)
            
        u.close()
        t.close()

        self.recipes = data.split(';')

    def update_db(self):
        # Backup system
        r = open("nosalty.json", "r")
        backup = r.readlines()
        r.close()
        w = open("nosalty.json.backup", "w")
        w.writelines(backup)
        w.flush()
        w.close()

        # Update system
        w = open("nosalty.json", "w")
        self.recipes.remove(self.recipes[len(self.recipes) - 1])
        for recipe in self.recipes:
            data = json.loads(recipe.replace("'", '"'))
            w.write(str(data) + "\n")
            w.flush()
            print (data["name"])
        w.close()
        
print (f"Program started at {begin}\n")
recipes = GetRecipeSites()
recipes.run()
recipes_urls = recipes.get_list_of_urls()
print ("\nFinished gathering recipes urls...")
recipe_data = GetRecipeData(recipes_urls)
print ("\nStarting to receive recipe data...")
recipe_data.run()
print ("\n\nUpdating local Database...\n")
recipe_data.update_db()


end = time.time()

print (f"\nTotal runtime: {end - begin} seconds\n")