# For regular expressions
import re
# Used to open and request pages
import urllib2
# Beautiful Soup 4, allows navigation through a parse tree based in a page
from bs4 import BeautifulSoup

# Used to compose page links (home + relative address)
imdb = 'http://www.imdb.com'
# List of pages that will be read
series_list_pages = []
# First page with 50 series
series_list_pages.append ('http://www.imdb.com/search/title?title_type=tv_series')
# Second page with another 50
series_list_pages.append ('http://www.imdb.com/search/title?end=100&start=51&title_type=tv_series')

# Creating or truncating (if the file already exists) the text file
# and using it in this block as output_file. When this block ends
# the file is closed.
with open ('series.txt', 'w+') as output_file:
	# For each page with 50 series
	for list_page in series_list_pages:
		# Requesting a page of the list with the default User-agent
		req = urllib2.Request (list_page)
		# A file-like page received by the request
		page = urllib2.urlopen (req)
		# The parse-structure object using the html parser (parse-tree)
		soup = BeautifulSoup (page, 'html.parser')

		# The 50 series listed in this page are in '<table class="results"...'
		# and this table is found with this line
		table = soup.find ('table', { 'class' : 'results' })
		# Each of the 50 series has an image to illustrate it and each image has
		# a link to a serie's page
		series_images = table.find_all ('td', { 'class' : 'image' })

		# Those links will be saved in a list of links
		series_links = []
		#For each image in the list of images
		for serie_image in series_images:
			# There is a link '<a href=[serie's page]>[serie's title]</a>'
			serie_image = serie_image.find ('a')
			# And the relative link, prefixed with the IMDb homepage, should
			# be saved for later visit
			series_links.append (imdb + str (serie_image.get ('href')))

		# Each of the links should be followed
		for serie in series_links:
			# So there is a request for each page
			req = urllib2.Request (serie)
			# that gives a webpage
			page = urllib2.urlopen (req)
			# that is parsed and transformed in a structure that might be navigated
			soup = BeautifulSoup (page, 'html.parser')

			# Then, the title might be in a div of class originalTitle if the
			# title was translated (to Brazilian Portuguese, for example)
			title = soup.find ('div', { 'class' : 'originalTitle' })
			if title == None:
				# or in a title h1 in the page that is already the original title
				title = soup.find ('h1', { 'itemprop' : 'name' })
			# It doesn't mather the case, whether the title was translated or not
			# the original title is saved without additional spaces or tabs
			title = (title.find (text=True)).strip ()

			# The description of a serie is always in a div with itemprop='description'
			description = soup.find ('div', { 'itemprop' : 'description' })
			# and it is also saved without additional spaces or tabs
			description = (description.find (text=True)).strip ()

			# The country which produced a serie is nested in a div with details
			country = soup.find ('div', { 'id' : 'titleDetails' })
			# and in a relative link whose first characters are '/country'
			country = country.find ('a', { 'href' : re.compile ('^/country')})
			# The country name is also saved with no extra whitespaces or tabs
			country = (country.find (text=True)).strip ()

			# The release year is in a div whose class is title_wrapper
			release = soup.find ('div', { 'class' : 'title_wrapper' })
			# inside a link whose title is 'See more release dates'
			release = release.find ('a', { 'title' : 'See more release dates' })
			# As we just want to save the year in which the first episode was
			# released, and not the 'TV Series (' that comes before it
			# just the year part is saved
			release = (release.find (text=True))[11:15]

			# The seasons that already was released are descendent sorted
			# in a nav bar (menu) with links
			seasons = soup.find ('div', { 'class' : 'seasons-and-year-nav' })
			# There are for divs, two with titles: 'Seasons' and 'Years', the
			# third one with links to seasons and the last with links to the
			# years in which there will be episodes
			seasons = seasons.find_all ('div')
			# We just want the link in the third div
			seasons = seasons[2].find ('a')
			# And the text of it with no extra blank spaces
			seasons = (seasons.find (text=True)).strip ()

			# The genres of a serie are in the same div the title is
			genre = soup.find ('div', { 'class' : 'title_wrapper' })
			# There might be more than one of them but just the first is considered
                        genre = genre.find ('span', { 'class' : 'itemprop' }, { 'itemprop': 'genre'})
			# Again, spaces and tabs are not a rather have
			genre = (genre.find (text=True)).strip ()

			# Finally. all of the data gotten is printed in a file with tabs
			# separating them
			print >> output_file, title + '\t' \
			+ description + '\t' + country + '\t' \
			+ release + '\t' + seasons + '\t' + genre
			# And a new line for each record
	# Generally, having an empty newline at the end of the file is a convention
	print >> output_file, ''
