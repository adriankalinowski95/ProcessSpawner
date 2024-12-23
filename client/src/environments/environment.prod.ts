const isApiStaticAddress = false;

export const environment = {
    production: true,
    apiUrl: isApiStaticAddress ? 'http://192.168.1.190:5000' : 'http://localhost:5000',
    appVersion: 'v1.0',
    USERDATA_KEY: 'authf649fc9a5f55'
}