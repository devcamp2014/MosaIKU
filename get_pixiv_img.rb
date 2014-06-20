# -*- coding: utf-8 -*-

require 'uri'
require 'open-uri'
require 'csv'
require 'set'
require 'digest/sha1'

class PixivImg
  DOWNLOAD_LOG_FILE = "download.log"
  SESSION_ID = "11353907_6a9ca04b5e12727dee45049e2abce045"

  def initialize()
    @urlset = Set.new

    CSV.foreach(DOWNLOAD_LOG_FILE) do |row|
      @urlset.add(row[1])
    end

  end

  def search_url(keyword, page, sessionid)
    URI.escape("http://spapi.pixiv.net/iphone/search.php?&s_mode=s_tag&word=#{keyword}&order=date&PHPSESSID=#{sessionid}&p=#{page}")
  end

  def download_img(keyword, url)
    begin
      puts "download:#{url}"

      filename = Digest::SHA1.hexdigest(url) + File.extname(url)

      open("img/" + filename, 'wb') do |output|
        open(url, 'rb') do |input|
          output.write(input.read)
        end
      end

      CSV.open(DOWNLOAD_LOG_FILE, "ab") do |csv|
        csv << [keyword, url, filename]
      end
    rescue
      STDERR.puts "Error: #$!"
    end
  end

  public
  def get_images(keyword)
    (1..30).each do |page|
      #        downloaded = false
      puts "getting #{keyword}[#{page}]..."
      open(search_url(keyword, page, SESSION_ID)) do |f|
        CSV.parse(f.read).each do |e|
          url = e[6]
          unless @urlset.include?(url)
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
while true
  begin
    pi.get_images("初音ミク")
    pi.get_images("鏡音リン")
    pi.get_images("鏡音レン")
    pi.get_images("桜ミク")
    pi.get_images("雪ミク")
    pi.get_images("巡音ルカ")
    pi.get_images("弱音ハク")
    pi.get_images("MEIKO")
    pi.get_images("KAITO")
  rescue
  end
end
