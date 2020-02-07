import Adafruit_SSD1306

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

class Key(object):

	LEFT = 32
	TOP = 16

	def __init__(self):
		display = Adafruit_SSD1306.SSD1306_128_32(rst=None)
		display.begin()
		display.clear()
		display.display()
		self._display = display

		# it is a requirement that the image dimensions match the screen dimensions
		self._image = Image.new('1', (display.width, display.height))
		self._canvas = ImageDraw.Draw(self._image)

		try:
			self._font = ImageFont.truetype('FreeMono.ttf', 14)
		except IOError:
			self._font = ImageFont.load_default()
	@property
	def display(self):
		return 'nunya'

	# sets the string that will be displayed on the module
	# for our uses, this will probably be just a single character string
	@display.setter
	def display(self, value):
		self._canvas.rectangle((0, 0, self._display.width, self._display.height), outline=False, fill=False)
		self._canvas.text((Key.LEFT + 4, Key.TOP), value, font=self._font, fill=255)
		self._display.image(self._image)
		self._display.display()

key = Key()

while True:

	key.display = raw_input('What to display? ')
