#pragma once

#include "simple.h"
#include "simpleMath.h"
#include "simpleList.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::model::game
{
	struct color : hashable
	{

	public:

		// base RGB components: (0, 1) -> (0, 255)
		float red;
		float green;
		float blue;
		float alpha;

		color() 
		{
			red = 0;
			green = 0;
			blue = 0;
			alpha = 0;
		}
		color(const color& color)
		{
			red = color.red;
			green = color.green;
			blue = color.blue;
			alpha = color.alpha;
		}
		color(float ared, float agreen, float ablue, float aalpha)
		{
			red = ared;
			green = agreen;
			blue = ablue;
			alpha = aalpha;
		}
		void set(const color& copy)
		{
			red = copy.red;
			green = copy.green;
			blue = copy.blue;
			alpha = copy.alpha;
		}

		static color interpolate(const color& color1, const color& color2, float weight)
		{
			color result = color1;

			result.interpolate(color2, weight);

			return result;
		}

		void interpolate(const color& other, float weight)
		{
			red = (other.red * weight) + (red * (1 - weight));
			green = (other.green * weight) + (green * (1 - weight));
			blue = (other.blue * weight) + (blue * (1 - weight));
			alpha = (other.alpha * weight) + (alpha * (1 - weight));
		}

		void interpolate(float weight, const simpleList<color>& colors)
		{
			color seed(0, 0, 0, 0);

			colors.forEach([&seed] (color acolor)
			{
				seed += acolor;

				return iterationCallback::iterate;
			});

			seed.red /= colors.count();
			seed.green /= colors.count();
			seed.blue /= colors.count();
			seed.alpha /= colors.count();

			interpolate(seed, weight);
		}

		void averageIn(float selfWeight, float weight, const simpleList<color>& colors)
		{
			color seed(0, 0, 0, 0);

			colors.forEach([&seed] (color acolor)
			{
				seed += acolor;

				return iterationCallback::iterate;
			});

			red = (((red * selfWeight) + (seed.red * weight)) / 2.0f) / colors.count();
			green = (((green * selfWeight) + (seed.green * weight)) / 2.0f) / colors.count();
			blue = (((blue * selfWeight) + (seed.blue * weight)) / 2.0f) / colors.count();
			alpha = (((alpha * selfWeight) + (seed.alpha * weight)) / 2.0f) / colors.count();
		}

		template<typename T = color, typename ... TRest>
		void averageIn(float selfWeight, float weight, const color& next, const TRest&...rest)
		{
			color seed = averageImpl(next, rest...);

			red = ((red * selfWeight) + (seed.red * weight)) / 2.0f;
			green = ((green * selfWeight) + (seed.green * weight)) / 2.0f;
			blue = ((blue * selfWeight) + (seed.blue * weight)) / 2.0f;
			alpha = ((alpha * selfWeight) + (seed.alpha * weight)) / 2.0f;
		}

		template<typename T = color, typename ... TRest>
		void fromAverage(float weight, const color& next, const TRest&...rest)
		{
			color seed = averageImpl(next, rest...);

			red = weight * seed.red;
			green = weight * seed.green;
			blue = weight * seed.blue;
			alpha = weight * seed.alpha;
		}

	private:

		template<typename T = color, typename ... TRest>
		color averageImpl(const color& next, const TRest&...rest)
		{
			color seed(0, 0, 0, 0);

			sum(seed, next, rest...);

			int numberColors = 1 + sizeof...(rest);

			seed.red = seed.red / numberColors;
			seed.green = seed.green / numberColors;
			seed.blue = seed.blue / numberColors;
			seed.alpha = seed.alpha / numberColors;

			return seed;
		}

		template<typename T = color, typename ... TRest>
		void sum(color& output, const color& next, const TRest&...others)
		{
			output.red += next.red;
			output.green += next.green;
			output.blue += next.blue;
			output.alpha += next.alpha;

			if (sizeof...(others) > 0)
				sum(output, others...);
		}

		template<typename T = color>
		void sum(color& output, const color& other)
		{
			output.red += other.red;
			output.green += other.green;
			output.blue += other.blue;
			output.alpha += other.alpha;
		}

	public:

		float magnitude() const
		{
			return (red + green + blue + alpha) / 4.0f;
		}

		void operator=(const color& color)
		{
			red = color.red;
			green = color.green;
			blue = color.blue;
			alpha = color.alpha;
		}
		void operator*=(const color& color)
		{
			red = red * color.red;
			green = green * color.green;
			blue = blue * color.blue;
			alpha = alpha * color.alpha;
		}
		void operator+=(const color& color)
		{
			red += color.red;
			green += color.green;
			blue += color.blue;
			alpha += color.alpha;
		}
		void clamp(color low, color high)
		{
			red = simpleMath::clamp(red, low.red, high.red);
			green = simpleMath::clamp(green, low.green, high.green);
			blue = simpleMath::clamp(blue, low.blue, high.blue);
			alpha = simpleMath::clamp(alpha, low.alpha, high.alpha);
		}
		void clamp(float low, float high)
		{
			red = simpleMath::clamp(red, low, high);
			green = simpleMath::clamp(green, low, high);
			blue = simpleMath::clamp(blue, low, high);
			blue = simpleMath::clamp(alpha, low, high);
		}
		bool compare(const color& color) const
		{
			return red == color.red &&
					green == color.green &&
					blue == color.blue &&
					alpha == color.alpha;
		}
		bool operator ==(const color& other) const
		{
			return compare(other);
		}
		bool operator !=(const color& other) const
		{
			return !compare(other);
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(red, green, blue);
		}
	};

	struct colors
	{
	public:
		static color transparent()  { return color(0, 0, 0, 0); };
		static color black()		{ return color(0, 0, 0, 1); };
		static color red()			{ return color(1, 0, 0, 1); };
		static color orange()		{ return color(1, 0.5, 0, 1); };
		static color yellow()		{ return color(1, 1, 0, 1); };
		static color green()		{ return color(0, 1, 0, 1); };
		static color blue()			{ return color(0, 0, 1, 1); };
		static color purple()		{ return color(1, 0, 1, 1); };
		static color white()		{ return color(1, 1, 1, 1); };

	public:

		/// <summary>
		/// Returns a shade of gray with the specified value
		/// </summary>
		static color getGray(float value)
		{
			return color(value, value, value, 1.0);
		}
	};
}