import serial, time
import gspread
from oauth2client.service_account import ServiceAccountCredentials

def connect_to_drive_and_open_gspread():
	scope = ['https://spreadsheets.google.com/feeds']

	credentials = ServiceAccountCredentials.from_json_keyfile_name('pollution-6561beaef0c0.json', scope)

	gc = gspread.authorize(credentials)
	
	wks = gc.open("Pollution").sheet1
	return wks

def parsed_output(serial_output):
	output=serial_output.split(';')
	output=output[0]
	output=output.split(',')
	if len(output)<6:
		node_id=output[0]
		pkt_number=output[1]
		temp=output[2]
		co=output[3]
		co2=output[4]
		return node_id,pkt_number,temp,co,co2
	else:
		return 0

def write_cell(spread_sheet, cell, content):
	spread_sheet.update_acell(cell,content)

def is_written(cell):
	return 1


ser = serial.Serial('/dev/ttyUSB0', 115200)
wks=connect_to_drive_and_open_gspread()

starting_row=5;
col_nodeID='A';
col_pkt_num='B';
col_ts='C';
col_CO='D';
col_CO2='E';
col_temp='F';
row=starting_row;
while True:
	line = line = ser.readline() 
	node_id,pkt_number,temp,co,co2=parsed_output(line)
	print node_id,pkt_number,temp,co,co2
	write_cell(wks, col_nodeID+str(row), node_id)
	write_cell(wks, col_pkt_num+str(row), pkt_number)
	write_cell(wks, col_CO+str(row), co)
	write_cell(wks, col_CO2+str(row), co2)
	write_cell(wks, col_temp+str(row), temp)
	row=row+1;
ser.close()
