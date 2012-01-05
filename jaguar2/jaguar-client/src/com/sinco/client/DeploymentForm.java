/*
 * DeploymentForm.java
 *
 * Created on September 2, 2008, 1:16 PM
 */
package com.sinco.client;

import com.sinco.client.model.Deployment;
import com.sinco.network.NetworkException;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.UnknownHostException;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

/**
 *
 * @author  cross
 */
public class DeploymentForm extends javax.swing.JInternalFrame {

    Deployment model;

    /** Creates new form DeploymentForm */
    public DeploymentForm() {
        initComponents();
        model = new Deployment();
        setSize(500, 500);
        setClosable(true);
        setResizable(true);
        setMaximizable(true);
        setIconifiable(true);
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jButton1 = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Deployment Utility");

        jButton1.setLabel("Deploy");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(72, 72, 72)
                .addComponent(jButton1)
                .addContainerGap(278, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(31, 31, 31)
                .addComponent(jButton1)
                .addContainerGap(243, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
    try {

        javax.swing.JFileChooser chooser = new JFileChooser();

        if (chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            File file = chooser.getSelectedFile();
            // Reading the xml file
            InputStream xml = new FileInputStream(file);

            BufferedReader reader = new BufferedReader(new InputStreamReader(xml));
            String line;
            StringBuffer xmltext = new StringBuffer();
            while ((line = reader.readLine()) != null) {
                xmltext.append(line);
            }
            reader.close();
            model.setXmlDefinition(xmltext.toString());
            model.deploy();

            JOptionPane.showMessageDialog(this, String.format("Deployment successed. New processdefinition id: %d", model.getProcessDefinition()));
        }
    } catch (UnknownHostException ex) {
        JOptionPane.showMessageDialog(this, ex.getMessage());
    } catch (NetworkException ex) {
        JOptionPane.showMessageDialog(this, ex.getMessage());
    } catch (IOException ex) {
        JOptionPane.showMessageDialog(this, ex.getMessage());
    }

}//GEN-LAST:event_jButton1ActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                new DeploymentForm().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton jButton1;
    // End of variables declaration//GEN-END:variables
}