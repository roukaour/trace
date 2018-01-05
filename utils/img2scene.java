import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class img2scene {
	public static void main(final String args[]) throws IOException {
		final String filename = args[0];
		final File file = new File(filename);
		final BufferedImage image = ImageIO.read(file);
		final int w = image.getWidth();
		final int h = image.getHeight();
		final double hw = (double)w / 2 - .5;
		final double hh = (double)h / 2 + .5;
		final double hz = w > h ? (double)w / 2 : (double)h / 2;
		System.out.println("[ Generated from " + filename + " by img2scene.java ]");
		System.out.println("image " + (w * 2) + " " + (h * 2));
		System.out.println("screen " + w + " " + h);
		System.out.println("eye " + hw + " " + hh + " " + hz);
		System.out.println("look " + hw + " " + hh + " 0");
		System.out.println("up 0 1 0");
		System.out.println("fov 90");
		System.out.println("ambient 1 1 1");
		for (int y = 0; y < h; y++) {
			final int iy = h - y;
			for (int x = 0; x < w; x++) {
				final int c = image.getRGB(x, y);
				final int r = (c & 0xFF0000) >> 16;
				final int g = (c & 0xFF00) >> 8;
				final int b = c & 0xFF;
				final double rd = (double)r / 0xFF;
				final double gd = (double)g / 0xFF;
				final double bd = (double)b / 0xFF;
				System.out.println("sphere pos " + x + " " +
					iy + " 0 ambient " + rd + " " + gd + " " +
					bd + " radius 1 end");
			}
		}
	}
}
