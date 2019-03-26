/**
 Creates a new ChartColor for given base and highlight color (other values are set by setting the alpha channel on these).
 @class
 @classdesc A color configuration for Chart.js charts.
 @param {string} - The base color in hex (must have the format `#FFFFFF`).
 @param {string} - The highlight color in hex (must have the format `#FFFFFF`).
 @since v0.1.0
 @example
 ```js
 var skyblue = new ChartColor('#3465a4', '#729fcf');
 ```
 */
function ChartColor(color, highlight) {
	/**
	 Creates rgba from given hex color.
	 @param {object} - hex color code
	 @param {object} - alpha value
	 @returns {string} rgba
	 */
	var hexToRGBA = function(hex, alpha) {
		var result = 'rgba(_r, _g, _b, _a)';
		result = result.replace('_r', parseInt(hex.substr(1, 2), 16));
		result = result.replace('_g', parseInt(hex.substr(3, 2), 16));
		result = result.replace('_b', parseInt(hex.substr(5, 2), 16));
		result = result.replace('_a', alpha);
		return result;
	};

	/**
	 The base color.
	 @default color
	 @type {string}
	 */
	this.color = color;
	/**
	 The highlight color.
	 @default highlight
	 @type {string}
	 */
	this.highlight = highlight;
	/**
	 The fill color.
	 @default color with alpha channel 0.25.
	 @type {string}
	 */
	this.fillColor = hexToRGBA(color, 0.25);
	/**
	 The stroke color.
	 @default color with alpha channel 1.
	 @type {string}
	 */
	this.strokeColor = hexToRGBA(color, 1);
	/**
	 The point color.
	 @default color with alpha channel 1.
	 @type {string}
	 */
	this.pointColor = hexToRGBA(color, 1);
	/**
	 The point stroke color.
	 @default
	 @type {string}
	 */
	this.pointStrokeColor = "#fff";
	/**
	 The point highlight fill color.
	 @default
	 @type {string}
	 */
	this.pointHighlightFill = "#fff";
	/**
	 The point highlight stroke color.
	 @default highlight with alpha channel 1.
	 @type {string}
	 */
	this.pointHighlightStroke = hexToRGBA(highlight, 1);
	/**
	 The highlight fill color.
	 @default highlight with alpha channel 0.75.
	 @type {string}
	 */
	this.highlightFill = hexToRGBA(highlight, 0.75);
	/**
	 The highlight stroke color.
	 @default highlight with alpha channel 1.
	 @type {string}
	 */
	this.highlightStroke = hexToRGBA(highlight, 1);
}