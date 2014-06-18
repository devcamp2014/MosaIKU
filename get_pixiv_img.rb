# -*- coding: utf-8 -*-

require 'uri'
require 'open-uri'
require 'csv'
require 'set'
require 'digest/sha1'

class PixivImg
  DOWNLOAD_LOG_FILE = "download.log"

  def initialize()
    @urlset = Set.new

    CSV.foreach(DOWNLOAD_LOG_FILE) do |row|
      @urlset.add(row[1])
    end
  end

  def search_url(keyword, page)
    URI.escape("http://spapi.pixiv.net/iphone/search.php?&s_mode=s_tag&word=#{keyword}&order=date&PHPSESSID=0&p=#{page}")
  end

  def download_img(keyword, url)
    filename = Digest::SHA1.hexdigest(url) + File.extname(url)

    open("img/" + filename, 'wb') do |output|
      open(url, 'rb') do |input|
        output.write(input.read)
      end
    end

    CSV.open(DOWNLOAD_LOG_FILE, "ab") do |csv|
      csv << [keyword, url, filename]
    end
  end

  public
  def get_images(keyword)
    (1..5).each do |page|
      #        downloaded = false
      open(search_url(keyword, page)) do |f|
        CSV.parse(f.read).each do |e|
          url = e[6]
          unless @urlset.include?(url)
            puts url
            @urlset.add(url)
            download_img(keyword, url)
            #             downloaded = true
            sleep 5 # ゆっくりダウンロードしないと多分怒られる
          end
        end
      end
      sleep 10
      #        break unless downloaded
    end
  end
end

pi = PixivImg.new
pi.get_images("初音ミク")
pi.get_images("鏡音リン")
pi.get_images("鏡音レン")
pi.get_images("桜ミク")
pi.get_images("雪ミク")
pi.get_images("巡音ルカ")
pi.get_images("弱音ハク")
