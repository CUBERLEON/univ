package rmi;

import database.Result;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface IDatabaseRemote extends Remote {

    Result query(String msg) throws RemoteException;
}