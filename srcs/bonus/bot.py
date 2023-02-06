import requests
from dotenv import load_dotenv
import base64
import os

load_dotenv()

# Spotify API endpoint for retrieving the access token
token_url = "https://accounts.spotify.com/api/token"

# Your Spotify client ID and client secret
client_id = os.getenv("CLIENT_ID")
client_secret = os.getenv("CLIENT_SECRET")
artist_name = os.getenv("ARTIST_NAME")

# # Request the access token
auth_header = base64.b64encode((client_id + ":" + client_secret).encode()).decode()
auth_response = requests.post(token_url, headers={"Authorization": "Basic " + auth_header}, data={"grant_type": "client_credentials"})


# # Get the access token
access_token = auth_response.json()["access_token"]

# # Use the access token to search for an artist
search_url = "https://api.spotify.com/v1/search"
search_response = requests.get(search_url, headers={"Authorization": "Bearer " + access_token}, params={"q": {artist_name}, "type": "artist", "limit": 1})

# # Get the artist ID
artist_id = search_response.json()["artists"]["items"][0]["id"]

# Use the artist ID to retrieve the top tracks
tracks_url = "https://api.spotify.com/v1/artists/" + artist_id + "/top-tracks"
tracks_response = requests.get(tracks_url, headers={"Authorization": "Bearer " + access_token}, params={"country": "US"})

# Print the top 5 tracks
f = open("songs", "w")

top_tracks = tracks_response.json()["tracks"][:5]
enumerated_tracks = list(enumerate(top_tracks))
for i, track in enumerate(top_tracks):
    f.write(track['name'].encode('UTF-8') + "\n")
    
