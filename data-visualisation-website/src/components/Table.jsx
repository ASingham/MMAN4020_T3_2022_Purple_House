import Box from '@mui/material/Box';
import { DataGrid } from '@mui/x-data-grid';

const Table = ({ data, maxTemp }) => {
  const columns = [
    {
      field: 'date',
      headerName: 'Date',
      flex: 1,
      editable: false,
      type: 'date'
    },
    {
      field: 'time',
      headerName: 'Time',
      flex: 1,
      editable: false,
      type: 'number'
    },
    {
      field: 'temp',
      headerName: 'Temperature (°C)',
      flex: 2,
      editable: false,
      type: 'number',
      valueFormatter: ({ value }) => value ? `${value} °C` : ''
    }
  ];

  return (
    <>
      { data.length > 0 &&
        <Box sx={{ width: '100%', height: 600 }}>
          <DataGrid
            rows={data}
            columns={columns}
            rowsPerPageOptions={[100]}
            getRowClassName = {params => params.row.temp >= maxTemp ? 'highlight' : ''}
            initialState={{
              sorting: {
                sortModel: [{field: 'id', sort: 'asc'}]   // No multiple field sorting with free (community) version
              }
            }}
            sx={{
              '.highlight': {
                color: '#d32f2f',
                '&:hover': {
                  color: '#e57373'
                }
              }
            }}
          />
        </Box>
      }
    </>
  );
};

export default Table;
