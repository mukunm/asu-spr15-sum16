package indexStructures;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

public class ExHashTableops<K, V> {
	static class Page<K, V> {
		static int PAGE_SZ = 20;
		private Map<K, V> m = new HashMap<K, V>();
		int d = 0;

		boolean full() {
			return m.size() > PAGE_SZ;
		}

		void put(K k, V v) {
			m.put(k, v);

		}

		V get(K k) {
			return m.get(k);
		}
	}

	int gd = 0;

	List<Page<K, V>> pp = new ArrayList<Page<K, V>>();

	public ExHashTableops() {
		pp.add(new Page<K, V>());
	}

	Page<K, V> getPage(K k) {
		int h = k.hashCode();
		Page<K, V> p = pp.get(h & ((1 << gd) - 1));
		return p;
	}

	void put(K k, V v) {
		Page<K, V> p = getPage(k);
		if (p.full() && p.d == gd) {
			List<Page<K, V>> pp2 = new ArrayList<ExHashTableops.Page<K, V>>(pp);
			pp.addAll(pp2);
			++gd;
		}

		if (p.full() && p.d < gd) {
			p.put(k, v);
			Page<K, V> p1, p2;
			p1 = new Page<K, V>();
			p2 = new Page<K, V>();
			for (K k2 : p.m.keySet()) {
				V v2 = p.m.get(k2);

				int h = k2.hashCode() & ((1 << gd) - 1);

				if ((h | (1 << p.d)) == h)
					p2.put(k2, v2);
				else
					p1.put(k2, v2);
			}

			List<Integer> l = new ArrayList<Integer>();

			for (int i = 0; i < pp.size(); ++i)
				if (pp.get(i) == p)
					l.add(i);

			for (int i : l)
				if ((i | (1 << p.d)) == i)
					pp.set(i, p2);
				else
					pp.set(i, p1);

			p1.d = p.d + 1;
			p2.d = p1.d;

		} else
			p.put(k, v);
	}

	public V get(K k) {
		return getPage(k).get(k);
	}

	public static void main(String[] args) {

		int N = 500000;

		Random r = new Random();
		List<Integer> l = new ArrayList<Integer>();
		for (int i = 0; i < N; ++i) {
			l.add(i);
		}

		for (int i = 0; i < N; ++i) {
			int j = r.nextInt(N);
			int t = l.get(j);
			l.set(j, l.get(i));
			l.set(i, t);
		}

		ExHashTableops<Integer, Integer> eh = new ExHashTableops<Integer, Integer>();
		for (int i = 0; i < N; ++i) {
			eh.put(l.get(i), l.get(i));
		}

		for (int i = 0; i < N; ++i) {
			System.out.printf("%d:%d , ", i, eh.get(i));
			if (i % 10 == 0)
				System.out.println();
		}

	}
}